#include "../inc/client.h"

#define REQUEST_HISTORY 10

GtkWidget *chatWindow;
GtkWidget *sendEntry, *sendButton;
GtkWidget *statusLabel;
GtkWidget *Viewport;
GtkAdjustment *vAdjust;
GtkScrolledWindow *scrolledWindow;
GtkListBox *messageList;
pthread_t watcher;
pthread_t server;
gulong hendler_id_entry;
gulong hendler_id_button;

int mx_show_popup(GtkWidget *widget, GdkEvent *event) {
	if (event->type == GDK_BUTTON_PRESS) {

		GdkEvent *bevent = (GdkEvent *) event;

		gtk_menu_popup_at_pointer(GTK_MENU(widget), bevent);

		return TRUE;
	}

	return FALSE;
}

void *mx_find_row_from_list__row(void *widget) {
	for (t_list_node *node = history_message_list->head; node; node = node->next) {

		if (widget == (void *)(((t_user_message *)(node->data))->row))
			return (t_user_message *)(node->data);
	}
	return NULL;
}

void mx_delete_mess(GtkWidget *widget, gpointer data) {
	widget = NULL;
	if (is_editing)
		return;
	t_user_message *current_message = mx_find_row_from_list__row(data);

	if (current_message) {
		mx_do_message_request(current_message, "delete_message");
	}
}

void mx_edit_message_complite(GtkWidget *widget, gpointer data) {
	(void) widget;
	char *new_mess = (char *)gtk_entry_get_text(GTK_ENTRY(sendEntry));
	t_user_message *temp = mx_create_edit_message((t_user_message *)data, new_mess);
	if (temp) {
		mx_do_message_request(temp, "update_message");
		is_editing = false;
	}
}

void mx_edit_mess(GtkWidget *widget, gpointer data) {
	is_editing = true;
	t_user_message *current_message = mx_find_row_from_list__row(data);

	widget = NULL;
	gtk_entry_set_text(GTK_ENTRY(sendEntry), current_message->data);
	g_signal_handler_disconnect(sendEntry, hendler_id_entry);
	g_signal_handler_disconnect(sendButton, hendler_id_button);
	hendler_id_entry = g_signal_connect(G_OBJECT(sendEntry),"activate", G_CALLBACK(mx_edit_message_complite), current_message);
	hendler_id_button = g_signal_connect(G_OBJECT(sendButton),"clicked", G_CALLBACK(mx_edit_message_complite), current_message);
}

void mx_listbox_node_cleaning(t_list_node *node){
	t_user_message *temp = ((t_user_message *)(node->data));
	if (temp && temp->row) {
		gtk_widget_destroy(temp->row);
		temp->row = NULL;
	}
}

gboolean mx_listbox_cleaning(){
	lock = true;
	mx_foreach_list(history_message_list, mx_listbox_node_cleaning);
	lock = false;
	return FALSE;
}

char *mx_short_time(time_t *t) {
    char *s;
    if ((time(0) - *t) < 86400 && (time(0) - *t) >= 0)
        s = mx_strndup((ctime)(t) + 11, 5);
    else {
        s = mx_strndup((ctime)(t) + 4, 12);
        // s = mx_strncat(s, ((ctime)(t) + 19), 5);
    }
    return s;
}

GtkWidget *mx_create_in_mess(const char *message_text, const char *login_text, time_t time_v) {
	GtkWidget *row;
	GtkWidget *in_row;
	GtkWidget *nickname;
	GtkWidget *time;
	GtkWidget *message_label;
	GtkWidget *message;
	GtkWidget *void_box;
	GtkWidget *container;

	row = gtk_list_box_row_new ();

    gtk_widget_show(row);
	gtk_list_box_row_set_activatable(GTK_LIST_BOX_ROW(row), FALSE); // !
	gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(row), FALSE); // !

	in_row = gtk_box_new (GTK_ORIENTATION_VERTICAL, TRUE);
    gtk_widget_show(in_row);
	nickname = gtk_label_new(login_text);
    gtk_widget_show(nickname);
	gtk_widget_set_halign (nickname, GTK_ALIGN_START);
	message_label = gtk_label_new(message_text);
    gtk_widget_show(message_label);
	gtk_label_set_line_wrap(GTK_LABEL(message_label), TRUE);
	message = gtk_button_new();
    gtk_widget_show(message);
	gtk_container_add (GTK_CONTAINER(message), message_label);
	time = gtk_label_new(mx_short_time(&time_v));
    gtk_widget_show(time);
	void_box = gtk_box_new(FALSE, 0);
    gtk_widget_show(void_box);
	gtk_container_add (GTK_CONTAINER(void_box), time);
	gtk_box_set_baseline_position(GTK_BOX(void_box), GTK_BASELINE_POSITION_CENTER);

	gtk_container_add (GTK_CONTAINER(in_row), message);
	gtk_container_add (GTK_CONTAINER(in_row), nickname);

	container = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, TRUE);
    gtk_widget_show(container);

	gtk_container_add (GTK_CONTAINER(container), in_row);
	gtk_container_add (GTK_CONTAINER(container), void_box);

	gtk_container_add (GTK_CONTAINER(row), container);
	gtk_widget_set_halign (row, GTK_ALIGN_START);

	gtk_widget_set_name(message, "input_message");
	gtk_widget_set_name(nickname, "nickname");
	gtk_widget_set_name(time, "time");
	gtk_widget_set_name(message_label, "message");

	return row;
}

GtkWidget *mx_create_out_mess(const char *message_text, const char *login_text, time_t time_v){
	GtkWidget *popup_menu;
	GtkWidget *row;
	GtkWidget *in_row;
	GtkWidget *nickname;
	GtkWidget *time;
	GtkWidget *message_label;
	GtkWidget *message;
	GtkWidget *void_box;
	GtkWidget *container;
	GtkWidget *edit;
	GtkWidget *delete;
	GtkWidget *ebox;

	popup_menu = gtk_menu_new();

	row = gtk_list_box_row_new ();

    gtk_widget_show(row);
	gtk_list_box_row_set_activatable(GTK_LIST_BOX_ROW(row), FALSE); // !
	gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(row), FALSE); // !

	in_row = gtk_box_new (GTK_ORIENTATION_VERTICAL, TRUE);
    gtk_widget_show(in_row);
	nickname = gtk_label_new(login_text);
    gtk_widget_show(nickname);
	gtk_widget_set_halign (nickname, GTK_ALIGN_END);
	message_label = gtk_label_new(message_text);
    gtk_widget_show(message_label);
	gtk_label_set_line_wrap(GTK_LABEL(message_label), TRUE);
	message = gtk_button_new();
    gtk_widget_show(message);
	gtk_container_add (GTK_CONTAINER(message), message_label);
	time = gtk_label_new(mx_short_time(&time_v));
    gtk_widget_show(time);
	void_box = gtk_box_new(FALSE, 0);
    gtk_widget_show(void_box);
	gtk_container_add (GTK_CONTAINER(void_box), time);
	gtk_box_set_baseline_position(GTK_BOX(void_box), GTK_BASELINE_POSITION_CENTER);

	ebox = gtk_event_box_new();
    gtk_widget_show(ebox);

	gtk_container_add (GTK_CONTAINER(ebox), message);
	gtk_container_add (GTK_CONTAINER(in_row), ebox);
	gtk_container_add (GTK_CONTAINER(in_row), nickname);

	container = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, TRUE);
    gtk_widget_show(container);

	gtk_container_add (GTK_CONTAINER(container), void_box);
	gtk_container_add (GTK_CONTAINER(container), in_row);

	edit = gtk_menu_item_new_with_label("Edit");
	gtk_menu_item_activate(GTK_MENU_ITEM(edit));
	gtk_widget_show(edit);
	gtk_menu_shell_append(GTK_MENU_SHELL(popup_menu), edit);

	delete = gtk_menu_item_new_with_label("Delete");
	gtk_menu_item_activate(GTK_MENU_ITEM(delete));
	gtk_widget_show(delete);
	gtk_menu_shell_append(GTK_MENU_SHELL(popup_menu), delete);

	g_signal_connect_swapped(G_OBJECT(ebox), "button-press-event",
	G_CALLBACK(mx_show_popup), popup_menu);

	gtk_container_add (GTK_CONTAINER(row), container);
	gtk_widget_set_halign (row, GTK_ALIGN_END);

	gtk_widget_set_name(message, "output_message");
	gtk_widget_set_name(row, "row");
	gtk_widget_set_name(edit, "edit_tool");
	gtk_widget_set_name(delete, "edit_tool");
	gtk_widget_set_name(popup_menu, "popup_menu");
	gtk_widget_set_name(nickname, "nickname");
	gtk_widget_set_name(time, "time");
	gtk_widget_set_name(message_label, "message");

	g_signal_connect(G_OBJECT(delete),"activate", G_CALLBACK(mx_delete_mess), row);
	g_signal_connect(G_OBJECT(edit),"activate", G_CALLBACK(mx_edit_mess), row);

	return row;
}
// void static print_history(t_list *history_message_list);

void mx_messdel(t_user_message **message) {
	if (message && *message) {
		mx_strdel((char**)&((*message)->nickname));
    	mx_strdel(&((*message)->data));
    	free(*message);
    	*message = NULL;
	}
}

t_user_message *mx_create_edit_message(t_user_message *message, char *msg_body) {
	t_user_message *new = NULL;

	if (message) {
		new = (t_user_message *)malloc(sizeof(t_user_message));
		new->owner_id = message->owner_id;
		new->data = strdup(msg_body);
		new->tv_id = message->tv_id;
		new->nickname = message->nickname;
	}
	return new;
}

bool mx_is_message_by_data(t_list_node *node, int owner_id, int message_id, t_user_message **message) {
	if (message)
		*message = NULL;

	if (((t_user_message *)(node->data))->tv_id == message_id
		&& ((t_user_message *)(node->data))->owner_id == (unsigned int)owner_id) {
		if (message) {
			*message = (node->data);
		}
		return true;
	}
	return false;
}

int mx_get_index_history_message(t_list *list, int owner_id, int message_id, t_user_message **message) {
	t_list_node *node = list ? list->head : NULL;
	int i = 0;

   	for (; node; node = node->next, i++)
   		if (mx_is_message_by_data(node, owner_id, message_id, message))
       		return i;
    return -1;
}

t_user_message *mx_proc_message_back(json_object *jobj) { // do free data
    int error = json_object_get_int(json_object_object_get(jobj, "error"));
    int user_id = json_object_get_int(json_object_object_get(jobj, "user_id"));
    char *user_nickname = strdup((char *)json_object_get_string(json_object_object_get(jobj, "user_nickname")));
    int time_id = json_object_get_int(json_object_object_get(jobj, "msg_time"));
    char *msg_body = strdup((char *)json_object_get_string(json_object_object_get(jobj, "msg_body")));
    t_user_message *message = NULL;

    if (error == 0 && msg_body && *msg_body) {
    	message = (t_user_message *)malloc(sizeof(t_user_message));
	    printf("\nMESSAGE from %s\t:::\t\" %s \"\n", user_nickname, msg_body);
	    message->owner_id = user_id;
	    message->data = msg_body;
	    message->tv_id = time_id;
	    message->nickname = user_nickname;
	    message->row = NULL;
	}
	if (message) {
		printf("DEBUG_MESSAGE: %d, %s, %lu, %s\n", message->owner_id, message->data, message->tv_id, message->nickname);
	}
    return message;
}

void mx_do_message_request(t_user_message *message, const char *request) {
	char *data = NULL;

	if (!message) {
		return;
	}
	json_object *jobj = json_object_new_object();
	json_object *j_type = json_object_new_string(request);
	json_object *j_mess = json_object_new_string(message->data);
	json_object *j_id = json_object_new_int(message->owner_id);
	json_object *j_msg_id = json_object_new_int((int)(message->tv_id));
	json_object *j_nick = json_object_new_string(message->nickname);

	json_object_object_add(jobj, "type", j_type);
	json_object_object_add(jobj, "msg_body", j_mess);
	json_object_object_add(jobj, "msg_time", j_msg_id);
	json_object_object_add(jobj, "user_id", j_id);
	json_object_object_add(jobj, "user_nickname", j_nick);
	data = (char *)json_object_to_json_string(jobj);
	printf("CLIENT->SERVER: %s", data);                                            	/***************/
	if (tls_write(tls_ctx, data, strlen(data)) == -1) {
		printf("error = %s\n", strerror(errno));
	}
}

void *mx_watcher_thread(void *param)
{
	mx_do_message_request((t_user_message *)param, "new_message");
	return param;
}

gboolean mx_add_message_widget(t_user_message *new_message) {
	GtkWidget *temp = NULL;

	if (!new_message)
		return FALSE;
	if(new_message->owner_id == owner.id){
    	temp = mx_create_out_mess(new_message->data, new_message->nickname, new_message->tv_id);
    }
    else {
    	temp = mx_create_in_mess(new_message->data, new_message->nickname, new_message->tv_id);
    }
    new_message->row = temp;
    gtk_widget_show(temp);
    gtk_list_box_insert(messageList, temp, -1);
    // gtk_adjustment_set_value(vAdjust, gtk_adjustment_get_upper(vAdjust));
    return FALSE;
}

void mx_delete_message_row(t_user_message *message) {
	if (message && message->row) {
		gtk_widget_destroy(message->row);
		message->row = NULL;
	}

}


gboolean mx_edit_message(t_edit_data *edit) {
	if (edit->message && edit->message->row && edit->new_message){

    	mx_delete_message_row(edit->message);
		if (edit->message->owner_id == owner.id)
			edit->message->row = mx_create_out_mess(edit->new_message->data, edit->new_message->nickname, edit->new_message->tv_id);
		else
			edit->message->row = mx_create_in_mess(edit->new_message->data, edit->new_message->nickname, edit->new_message->tv_id);
		printf("%d\n", edit->index);
		gtk_widget_show(edit->message->row);
		gtk_list_box_insert(messageList, edit->message->row, edit->index);

		gtk_entry_set_text(GTK_ENTRY(sendEntry), "");
		g_signal_handler_disconnect(sendEntry, hendler_id_entry);
		g_signal_handler_disconnect(sendButton, hendler_id_button);
		hendler_id_entry = g_signal_connect(G_OBJECT(sendEntry),"activate", G_CALLBACK(mx_do_send), NULL);
		hendler_id_button = g_signal_connect(G_OBJECT(sendButton),"clicked", G_CALLBACK(mx_do_send), NULL);
	}
	return FALSE;
}

gboolean mx_delete_message(t_user_message *new_message) {
	t_user_message *message = NULL;
	int index_msg = mx_get_index_history_message(history_message_list, new_message->owner_id, new_message->tv_id, &message);

    if (message && index_msg != -1) {
    	mx_pop_index(history_message_list, index_msg);
    	mx_delete_message_row(message);
    	mx_messdel(&message);
    }
    return FALSE;
}

void mx_switch_message_back(t_user_info *user, t_user_message *new_message) {
	char *type = user->last_server_back;
	t_user_message *message = NULL;
	int index_msg = 0;
	t_edit_data *edit = (t_edit_data *)malloc(sizeof(t_edit_data));

    if (strcmp(type, "new_message_back") == 0) {
    	mx_push_back(history_message_list, new_message);
    	gdk_threads_add_idle((GSourceFunc)mx_add_message_widget, new_message);
    	// mx_add_message_widget(new_message);
    }
    else if (strcmp(type, "update_message_back") == 0) {
    	index_msg = mx_get_index_history_message(history_message_list, new_message->owner_id, new_message->tv_id, &message);
    	printf("INDEX :::::::::::::::::::::: %d\n", index_msg);
    	message->data = strdup(new_message->data);
    	edit->message = message;
    	edit->new_message = new_message;
    	edit->index = index_msg;
    	gdk_threads_add_idle((GSourceFunc)mx_edit_message, edit);
    }
    else if (strcmp(type, "delete_message_back") == 0) {
    	gdk_threads_add_idle((GSourceFunc)mx_delete_message, new_message);
    }
}

void mx_change_title(char *name) {
	if(name == NULL){
		gtk_window_set_title(GTK_WINDOW(chatWindow), "Connection...");
	}
	else {
		gtk_window_set_title(GTK_WINDOW(chatWindow), name);
	}
}

int mx_do_reconnection(int rc) {
	t_client_info *info = get_client_info();
	int counter = 0;

	while(rc <= 0) {
		sleep(CNCT_CLDN);
		printf("TRY CONNECTING\n"); // выод на панели
		close(info->socket);
		if (tls_ctx){
			tls_close(tls_ctx);
			tls_free(tls_ctx);
			tls_ctx = NULL;
		}
		info->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (mx_init_connection(info->socket, start_data) == 0)
			return 0;
		if (counter++ == CNCT_AM)
			return -1;
	}
	return 0;
}

void mx_clear_node(t_list_node *node) {
	if (node) {
		mx_messdel((t_user_message **)(&node->data));
	}
	node->data = NULL;
}

void mx_clear_history(void) {
	mx_foreach_list(history_message_list, mx_clear_node);
	while (!history_message_list->head && !history_message_list->tail)
		mx_pop_back(history_message_list);
}

void *mx_read_server_thread(void *par) {
	char answ[BUF_SIZE];
	int tail = 0;
	t_user_message *message = NULL;

	while (true) {
		if ((tail = tls_read(tls_ctx, answ, BUF_SIZE)) <= 0) {
			gdk_threads_add_idle((GSourceFunc)mx_change_title, NULL);
			tail = mx_do_reconnection(tail);
			if (tail < 0)
				exit(1);
			lock = false;
    		gdk_threads_add_idle((GSourceFunc)mx_listbox_cleaning, NULL);
			for (int i = 0; i < CNCT_AM && mx_message_do_login(&owner); i++);

			gdk_threads_add_idle((GSourceFunc)mx_change_title, (void *)owner.nickname);
			while (lock) {};
			mx_clear_history();
			mx_message_request_history();
			continue;
		}
		answ[tail] = '\0';
		printf("SERVER THREAD: %s\n", answ);
		message = mx_proc_server_back(answ, &owner);
		if (!message)
			continue;
		mx_switch_message_back(&owner, message);
		message = NULL;
	}
	return par;
}

gboolean mx_autoscroll(){
    gtk_adjustment_set_value(vAdjust, gtk_adjustment_get_upper(vAdjust));
    return TRUE;
}

void mx_do_send() {
	char *mess_data;

	mess_data = strdup(gtk_entry_get_text(GTK_ENTRY(sendEntry)));
	if(!mess_data || !*mess_data)
	{
		gtk_widget_grab_focus(sendEntry);
		return;
	}

	t_user_message *message = (t_user_message *)malloc(sizeof(t_user_message)); // memleak possible
	message->owner_id = owner.id;
	message->data = mess_data;
	message->nickname = owner.nickname;
	message->tv_id = 0;
	pthread_create(&watcher, 0, mx_watcher_thread, (void *)message);
	gtk_entry_set_text(GTK_ENTRY(sendEntry), "");
	// printf("\n==============%f\n", gtk_adjustment_get_upper(vAdjust));
	gdk_threads_add_idle((GSourceFunc)mx_autoscroll, NULL);

}

void mx_zero_string(char *arr) {
	for (int i =0; i < BUF_SIZE; i++)
		arr[i] = '\0';
}

int mx_history_size(json_object *jobj) {
	const char *type = json_object_get_string(json_object_object_get(jobj, "type"));

	if (strcmp(type, "hst_size") == 0) {
		return json_object_get_int(json_object_object_get(jobj, "size"));
	}
	mx_strdel((char **)&type);
	return 0;
}

void mx_do_history_ready() {						//*
	tls_write(tls_ctx, "r", 1);									//	*	READY
}														//*

t_list *mx_create_hst_message_list(int list_size) {
	char buffer[BUF_SIZE];
	json_object *jobj;
	const char *type = NULL;
	t_user_message *message = NULL;
	short last = -1;

	for (int i = 0; i < list_size; ++i, message = NULL) {
		mx_do_history_ready();
		last = tls_read(tls_ctx, buffer, BUF_SIZE);
		if (last > 0) {
			buffer[last] = '\0';
			jobj = json_tokener_parse(buffer);
		}
		else
			continue;
		type = json_object_get_string(json_object_object_get(jobj, "type"));
		if (strcmp(type, "hst_list") == 0) {
			message = mx_proc_message_back(jobj);
			if (message) {
				mx_push_back(history_message_list, message);
				mx_add_message_widget(message);
			}
		}
		json_object_put(jobj);

	}
	return history_message_list;
}

/* ============== TEST HISTORY ================*/

// void static print_node(t_list_node *node) {
// 	static int i = 0;
// 	t_user_message *mess = (t_user_message *)(node->data);

// 	printf("\n=================================================================================================\n");
// 	printf("%d)\tMESSAGE FROM \"%s\"\t#%lu\tuser id: %d\t:::\t%s\n", i, mess->nickname, mess->tv_id, mess->owner_id, mess->data);
// 	printf("=================================================================================================\n");
// 	i++;
// }

// void static print_history(t_list *list) {
// 	system("clear");
// 	printf("\n\t\t\t::: HISTORY LIST :::\n");
// 	// mx_foreach_list(list, print_node);
// }

void mx_do_history_request() {
	const char *data;
	json_object *jobj = json_object_new_object();
	json_object *j_type = json_object_new_string("history_request");
	json_object_object_add(jobj, "type", j_type);

	data = json_object_to_json_string(jobj);
	tls_write(tls_ctx, data, strlen(data));
}

void mx_message_request_history(void) {
	char answ[BUF_SIZE];
	json_object *jobj;

	mx_do_history_request();
	mx_zero_string(answ); /////////// не обнулять а через рид
	tls_read(tls_ctx, answ, BUF_SIZE);
	if (strcmp("no_history", answ) == 0)
		return;
	jobj = json_tokener_parse(answ);
	int hst_size = mx_history_size(jobj);
	mx_create_hst_message_list(hst_size);
	json_object_put(jobj);
}

void mx_init_chat_window(char *nickname)
{
	GtkBuilder *builder = gtk_builder_new_from_file("client/gld/chat.glade");

	history_message_list = mx_create_list();
	chatWindow = GTK_WIDGET(gtk_builder_get_object(builder,"ChatWindow"));
	gtk_window_set_title(GTK_WINDOW(chatWindow), nickname);
	g_signal_connect(chatWindow,"destroy", G_CALLBACK(gtk_main_quit),NULL);
	is_editing = false;

	gtk_window_set_position(GTK_WINDOW(chatWindow), GTK_WIN_POS_CENTER);
	sendEntry = GTK_WIDGET(gtk_builder_get_object(builder,"SendEntry"));
	sendButton = GTK_WIDGET(gtk_builder_get_object(builder,"SendButton"));
	hendler_id_entry = g_signal_connect(G_OBJECT(sendEntry),"activate", G_CALLBACK(mx_do_send),NULL);
	hendler_id_button = g_signal_connect(G_OBJECT(sendButton),"clicked", G_CALLBACK(mx_do_send),NULL);
	Viewport = GTK_WIDGET(gtk_builder_get_object(builder,"Viewport"));
	scrolledWindow = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder,"ScrolledWindow"));
	vAdjust = gtk_scrolled_window_get_vadjustment(scrolledWindow);
	messageList = GTK_LIST_BOX(gtk_builder_get_object(builder,"MessageListBox"));
	
	mx_message_request_history();
	GtkCssProvider *cssStyle;
	cssStyle = gtk_css_provider_new();
	gtk_css_provider_load_from_path(cssStyle, "./client/src/style.css", NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
			                               GTK_STYLE_PROVIDER(cssStyle),
			                               GTK_STYLE_PROVIDER_PRIORITY_USER);

	gtk_widget_set_name(Viewport, "viewport");
	gtk_widget_set_name(chatWindow, "main_window");
	gtk_widget_set_name(sendButton, "send_button");
	gtk_widget_set_name(sendEntry, "send_entry");
	pthread_create(&server, 0, mx_read_server_thread, NULL);
}
