#include "../inc/client.h"

#define REQUEST_HISTORY 10

GtkWidget *chatWindow;
GtkWidget *sendEntry, *sendButton, *messageButton1, *messageButton2;
GtkWidget *messageBox1, *messageBox2;
GtkWidget *statusLabel;
GtkWidget *messagesTreeView;
GtkAdjustment *vAdjust;
GtkScrolledWindow *scrolledWindow;
GtkListStore *messagesListStore;
GtkListBox *messageList;
pthread_t watcher;
pthread_t server;
gulong hendler_id_entry;
gulong hendler_id_button;


// void *watcher_thread(void *param) {
// 	(void) param;
// 	scrolledWindow = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder,"scrolledWindow"));
// 	vAdjust = gtk_scrolled_window_get_vadjustment(scrolledWindow);
// 	pthread_create(&watcher, 0, watcher_thread, 0);
// }


int mx_show_popup(GtkWidget *widget, GdkEvent *event) {

	const gint RIGHT_CLICK = 3;

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

// t_list_node *mx_find_row_from_list__message(void *new_message) {
// 	for (t_list_node *node = row_history_list->head; node; node = node->next) {

// 		if (((t_user_message *)new_message)->tv_id == ((t_row *)(node->data))->message->tv_id
// 			&& ((t_user_message *)new_message)->owner_id == ((t_row *)(node->data))->message->owner_id)
// 			return (t_row *)(node->data);
// 	}
// 	return NULL;
// }

void mx_delete_mess(GtkWidget *widget, gpointer data){
	t_user_message *current_message = mx_find_row_from_list__row(data);

	if (current_message) {
		mx_do_message_request(current_message, "delete_message");
	}
}

void mx_edit_message_complite(GtkWidget *widget, gpointer data){
	// printf("DEBUG<<<<<<<<<<<<<<<<: %d, %s, %lu, %s\n", ((t_user_message *)data)->owner_id, ((t_user_message *)data)->data, ((t_user_message *)data)->tv_id, ((t_user_message *)data)->nickname);
	(void) widget;
	char *new_mess = (char *)gtk_entry_get_text(GTK_ENTRY(sendEntry));
	t_user_message *temp = mx_create_edit_message((t_user_message *)data, new_mess);
	// g_signal_handler_disconnect_by_func(widget, G_CALLBACK(mx_edit_message_complite), data);
	if (temp) {
		mx_do_message_request(temp, "update_message");
	}
	// g_signal_handler_disconnect_by_func(G_OBJECT(sendButton), mx_edit_message_complite, NULL);
}

void mx_edit_mess(GtkWidget *widget, gpointer data){
	printf("ХЕР\n");
	printf("*DATA*\t%p\n", data);
	t_user_message *current_message = mx_find_row_from_list__row(data);

	// if (!current_message)
	// 	printf("Ну нема тут ничего!\n");
	printf("БЬІЛ       %s\n", current_message->data);
	// printf("DEBUG<<<<<<<<<<<<<<<<: %d, %s, %lu, %s\n", message->owner_id, message->data, message->tv_id, message->nickname);
	gtk_entry_set_text(GTK_ENTRY(sendEntry), current_message->data);
	printf("ТАМ\n");
	// g_signal_handler_disconnect_by_func(G_OBJECT(sendButton), do_send, NULL);
	g_signal_handler_disconnect(sendEntry, hendler_id_entry);
	g_signal_handler_disconnect(sendButton, hendler_id_button);
	hendler_id_entry = g_signal_connect(G_OBJECT(sendEntry),"activate", G_CALLBACK(mx_edit_message_complite), current_message);
	hendler_id_button = g_signal_connect(G_OBJECT(sendButton),"clicked", G_CALLBACK(mx_edit_message_complite), current_message);
	// g_signal_connect_swapped(G_OBJECT(sendButton),"clicked", G_CALLBACK(mx_edit_message_complite),NULL);
}

GtkWidget *mx_create_in_mess(const char *message_text, const char *login_text){
	GtkWidget *popup_menu;
	GtkWidget *row;
	GtkWidget *in_row;
	GtkWidget *nickname;
	GtkWidget *time;
	GtkWidget *message_label;
	GtkWidget *message;
	GtkWidget *void_box;
	GtkWidget *container;
	GtkWidget *tool_box;
	// GtkWidget *edit;
	GtkWidget *delete;
	GtkWidget *ebox;
	tool_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, TRUE);
	// edit = gtk_button_new();
	delete = gtk_button_new();
	// gtk_container_add (GTK_CONTAINER(tool_box), edit);
	gtk_container_add (GTK_CONTAINER(tool_box), delete);

	popup_menu = gtk_menu_new();

	// GtkCssProvider *cssStyle2;
	// GtkCssProvider *cssStyle3;
	// cssStyle2 = gtk_css_provider_new();
	// cssStyle3 = gtk_css_provider_new();

	// gtk_css_provider_load_from_path(cssStyle2, "./client/src/input_mess_style.css", NULL);
	// gtk_css_provider_load_from_path(cssStyle3, "./client/src/other_mess_style.css", NULL);

	row = gtk_list_box_row_new ();

	gtk_list_box_row_set_activatable(GTK_LIST_BOX_ROW(row), FALSE); // !
	gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(row), FALSE); // !

	in_row = gtk_box_new (GTK_ORIENTATION_VERTICAL, TRUE);
	nickname = gtk_label_new(login_text);
	gtk_widget_set_halign (nickname, GTK_ALIGN_START);
	message_label = gtk_label_new(message_text);
	gtk_label_set_line_wrap(GTK_LABEL(message_label), TRUE);
	message = gtk_button_new();
	gtk_container_add (GTK_CONTAINER(message), message_label);
	time = gtk_label_new("10:20");
	void_box = gtk_box_new(FALSE, 0);
	gtk_container_add (GTK_CONTAINER(void_box), time);
	gtk_box_set_baseline_position(GTK_BOX(void_box), GTK_BASELINE_POSITION_CENTER);

	ebox = gtk_event_box_new();

	gtk_container_add (GTK_CONTAINER(ebox), message);
	gtk_container_add (GTK_CONTAINER(in_row), ebox);
	gtk_container_add (GTK_CONTAINER(in_row), nickname);

	container = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, TRUE);

	gtk_container_add (GTK_CONTAINER(container), in_row);
	gtk_container_add (GTK_CONTAINER(container), void_box);

	//   edit = gtk_menu_item_new_with_label("Edit");
	// gtk_widget_show(edit);
	// gtk_menu_shell_append(GTK_MENU_SHELL(popup_menu), edit);

	delete = gtk_menu_item_new_with_label("Delete");
	gtk_widget_show(delete);
	gtk_menu_shell_append(GTK_MENU_SHELL(popup_menu), delete);

	g_signal_connect_swapped(G_OBJECT(ebox), "button-press-event",
	G_CALLBACK(mx_show_popup), popup_menu);

	gtk_container_add (GTK_CONTAINER(row), container);
	gtk_widget_set_halign (row, GTK_ALIGN_START);

	gtk_widget_set_name(message, "row_to");
	// gtk_widget_set_name(edit, "edit");
	gtk_widget_set_name(delete, "edit");
	gtk_widget_set_name(popup_menu, "popup_menu");
	gtk_widget_set_name(nickname, "nickname");
	gtk_widget_set_name(time, "time");
	gtk_widget_set_name(tool_box, "popup_menu");
	gtk_widget_set_name(message_label, "message");

	// mx_css_set(cssStyle3, message);
	// // mx_css_set(cssStyle2, edit);
	// mx_css_set(cssStyle3, delete);
	// mx_css_set(cssStyle3, popup_menu);
	// mx_css_set(cssStyle3, nickname);
	// mx_css_set(cssStyle3, tool_box);
	// mx_css_set(cssStyle3, time);
	// mx_css_set(cssStyle3, message_label);

	return row;
}

void mx_create_new_container_for_row(const char *message_text, const char *login_text, GtkWidget **row) {
	GtkWidget *popup_menu;
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

	GtkCssProvider *cssStyle2;
	GtkCssProvider *cssStyle3;
	cssStyle2 = gtk_css_provider_new();
	cssStyle3 = gtk_css_provider_new();

	gtk_css_provider_load_from_path(cssStyle2, "./client/src/input_mess_style.css", NULL);
	gtk_css_provider_load_from_path(cssStyle3, "./client/src/other_mess_style.css", NULL);

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
	time = gtk_label_new("10:20");
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


	gtk_container_add(GTK_CONTAINER(*row), container);
	gtk_widget_set_halign (*row, GTK_ALIGN_END);

	gtk_widget_set_name(message_label, "message");


	mx_css_set(cssStyle2, message_label);
}

// GtkWidget *mx_create_out_mess(const char *message_text, const char *login_text){
// 	GtkWidget *popup_menu;
// 	GtkWidget *row;
// 	GtkWidget *in_row;
// 	GtkWidget *nickname;
// 	GtkWidget *time;
// 	GtkWidget *message_label;
// 	GtkWidget *message;
// 	GtkWidget *void_box;
// 	GtkWidget *container;
// 	GtkWidget *tool_box;
// 	GtkWidget *edit;
// 	GtkWidget *delete;
// 	GtkWidget *ebox;

// 	tool_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, TRUE);
// 	edit = gtk_button_new();
// 	delete = gtk_button_new();
// 	gtk_container_add (GTK_CONTAINER(tool_box), edit);
// 	gtk_container_add (GTK_CONTAINER(tool_box), delete);

// 	popup_menu = gtk_menu_new();

// 	GtkCssProvider *cssStyle2;
// 	GtkCssProvider *cssStyle3;
// 	cssStyle2 = gtk_css_provider_new();
// 	cssStyle3 = gtk_css_provider_new();

// 	gtk_css_provider_load_from_path(cssStyle2, "./client/src/input_mess_style.css", NULL);
// 	gtk_css_provider_load_from_path(cssStyle3, "./client/src/other_mess_style.css", NULL);

// 	row = gtk_list_box_row_new ();

//     gtk_widget_show(row);
// 	gtk_list_box_row_set_activatable(GTK_LIST_BOX_ROW(row), FALSE); // !
// 	gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(row), FALSE); // !

// 	in_row = gtk_box_new (GTK_ORIENTATION_VERTICAL, TRUE);
//     gtk_widget_show(in_row);
// 	nickname = gtk_label_new(login_text);
//     gtk_widget_show(nickname);
// 	gtk_widget_set_halign (nickname, GTK_ALIGN_END);
// 	message_label = gtk_label_new(message_text);
//     gtk_widget_show(message_label);
// 	gtk_label_set_line_wrap(GTK_LABEL(message_label), TRUE);
// 	message = gtk_button_new();
//     gtk_widget_show(message);
// 	gtk_container_add (GTK_CONTAINER(message), message_label);
// 	time = gtk_label_new("10:20");
//     gtk_widget_show(time);
// 	void_box = gtk_box_new(FALSE, 0);
//     gtk_widget_show(void_box);
// 	gtk_container_add (GTK_CONTAINER(void_box), time);
// 	gtk_box_set_baseline_position(GTK_BOX(void_box), GTK_BASELINE_POSITION_CENTER);

// 	ebox = gtk_event_box_new();
//     gtk_widget_show(ebox);

// 	gtk_container_add (GTK_CONTAINER(ebox), message);
// 	gtk_container_add (GTK_CONTAINER(in_row), ebox);
// 	gtk_container_add (GTK_CONTAINER(in_row), nickname);

// 	container = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, TRUE);
//     gtk_widget_show(container);

// 	gtk_container_add (GTK_CONTAINER(container), void_box);
// 	gtk_container_add (GTK_CONTAINER(container), in_row);

// 	edit = gtk_menu_item_new_with_label("Edit");
// 	gtk_menu_item_activate(GTK_MENU_ITEM(edit));
// 	gtk_widget_show(edit);
// 	gtk_menu_shell_append(GTK_MENU_SHELL(popup_menu), edit);

// 	delete = gtk_menu_item_new_with_label("Delete");
// 	gtk_menu_item_activate(GTK_MENU_ITEM(delete));
// 	gtk_widget_show(delete);
// 	gtk_menu_shell_append(GTK_MENU_SHELL(popup_menu), delete);

// 	g_signal_connect_swapped(G_OBJECT(ebox), "button-press-event",
// 	G_CALLBACK(show_popup), popup_menu);

// 	gtk_container_add (GTK_CONTAINER(row), container);
// 	gtk_widget_set_halign (row, GTK_ALIGN_END);

// 	gtk_widget_set_name(message, "row_to");
// 	gtk_widget_set_name(edit, "edit");
// 	gtk_widget_set_name(delete, "edit");
// 	gtk_widget_set_name(popup_menu, "popup_menu");
// 	gtk_widget_set_name(nickname, "nickname");
// 	gtk_widget_set_name(time, "time");
// 	gtk_widget_set_name(tool_box, "popup_menu");
// 	gtk_widget_set_name(message_label, "message");

// 	mx_css_set(cssStyle2, message);
// 	mx_css_set(cssStyle2, edit);
// 	mx_css_set(cssStyle2, delete);
// 	mx_css_set(cssStyle2, popup_menu);
// 	mx_css_set(cssStyle2, nickname);
// 	mx_css_set(cssStyle2, tool_box);
// 	mx_css_set(cssStyle2, time);
// 	mx_css_set(cssStyle2, message_label);

// 	g_signal_connect(G_OBJECT(delete),"activate", G_CALLBACK(mx_delete_mess), row);
// 	g_signal_connect(G_OBJECT(edit),"activate", G_CALLBACK(mx_edit_mess), row);

// 	return row;
// }
GtkWidget *mx_create_out_mess(const char *message_text, const char *login_text){

	// t_row temp = (t_row *)malloc(sizeof(t_row));

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

	GtkCssProvider *cssStyle2;
	GtkCssProvider *cssStyle3;
	cssStyle2 = gtk_css_provider_new();
	cssStyle3 = gtk_css_provider_new();

	gtk_css_provider_load_from_path(cssStyle2, "./client/src/input_mess_style.css", NULL);
	gtk_css_provider_load_from_path(cssStyle3, "./client/src/other_mess_style.css", NULL);

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
	time = gtk_label_new("10:20");
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

	gtk_widget_set_name(message, "row_to");
	gtk_widget_set_name(edit, "edit");
	gtk_widget_set_name(delete, "edit");
	gtk_widget_set_name(popup_menu, "popup_menu");
	gtk_widget_set_name(nickname, "nickname");
	gtk_widget_set_name(time, "time");
	gtk_widget_set_name(message_label, "message");

	mx_css_set(cssStyle2, message);
	mx_css_set(cssStyle2, edit);
	mx_css_set(cssStyle2, delete);
	mx_css_set(cssStyle2, popup_menu);
	mx_css_set(cssStyle2, nickname);
	mx_css_set(cssStyle2, time);
	mx_css_set(cssStyle2, message_label);

	g_signal_connect(G_OBJECT(delete),"activate", G_CALLBACK(mx_delete_mess), row);
	g_signal_connect(G_OBJECT(edit),"activate", G_CALLBACK(mx_edit_mess), row);

	return row;
}
void static print_history(t_list *history_message_list);

void mx_messdel(t_user_message **message) {
	// printf("\t\t:::\tMESSDEL START\n");
	if (message && *message) {
		// printf("\t\t:::\tMESSDEL IN IF\n");
		mx_strdel((char**)&((*message)->nickname));
		// printf("\t\t:::\tMESSDEL DELETED NICKNAME\n");
    	mx_strdel(&((*message)->data));
		// printf("\t\t:::\tMESSDEL DELETED DATA\n");
    	free(*message);
		// printf("\t\t:::\tMESSDEL DALETED NODE\n");
    	*message = NULL;
	}
	// printf("\t\t:::\tMESSDEL END\n");
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
		&& ((t_user_message *)(node->data))->owner_id == owner_id) {
		if (message) {
			*message = /*(t_user_message *)*/(node->data);
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



// int mx_get_index_history_message(t_list *list, int user_id, int message_id) {
// 	{
// 		t_list_node *node = list ? list->head : NULL;
// 		int i = 0;

//     	for (; node; node = node->next, i++)
//         	mx_is_message_by_data(node);
// 	}
// }

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
	char answ[BUF_SIZE];

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
	// read(clnt->sock, answ, BUF_SIZE);

	// mx_printstr("SERVER: ", 0);													/***************/
	// mx_printstr(answ, 0);
	// mx_printstr("\n", 0);
	// ///////////////////////////////////json_object_put(jobj);
	// system("leaks -q uchat");
	// printf("Aaaaaaaaaaaaaaaaaaa\n");
	// return mx_proc_server_mess(answ, reg_par);
}

void *watcher_thread(void *param)
{
	// (void) param;
	struct timeval tv;
	struct tm *nowtm;
	char *author = NULL, *body = NULL;
	char timebuf[64];
	mx_do_message_request((t_user_message *)param, "new_message");
	// mx_messdel((t_user_message **)&param);
	// message_request_history(REQUEST_HISTORY);
	// while(1)
	// {
	// 	// int k = message_receive(&tv, &author, &body);
	// 	// if(k < 0)
	// 	// {
	// 	//     gtk_label_set_text(GTK_LABEL(statusLabel), "Disconnected, please restart the client");
	// 	//     gtk_widget_set_sensitive(sendButton, 0);
	// 	//     break;
	// 	// }
	// 	// if(k == 0)
	// 	//     continue;
	// 	if(!author)
	// 	{
	// 		gtk_label_set_text(GTK_LABEL(statusLabel), body);
	// 		continue;
	// 	}
	// 	if(tv.tv_sec)
	// 	{
	// 		nowtm = localtime(&tv.tv_sec);
	// 		strftime(timebuf, 64, "[%d.%m.%Y %H:%M:%S]", nowtm);
	// 	}
	// 	else
	// 	{
	// 		*timebuf = 0;
	// 	}
	// 	// add_list_entry(timebuf, author, body, k != 'h');
	// 	free(author);
	// 	free(body);
	// }
	return param;
}

gboolean mx_add_message_widget(t_user_message *new_message) {
	GtkWidget *temp = NULL;

	if (!new_message)
		return FALSE;
	if(new_message->owner_id == owner.id){
    	temp = mx_create_out_mess(new_message->data, new_message->nickname);
    }
    else {
    	temp = mx_create_in_mess(new_message->data, new_message->nickname);
    }
		// printf("WIDGET\t\t:::\t%p\n", (void *)temp);
    new_message->row = temp;
    gtk_widget_show(temp);
    gtk_list_box_insert(messageList, temp, -1);
    return FALSE;
}

void mx_delete_message_row(t_user_message *message, int index) {
	GtkListBoxRow *temp = gtk_list_box_get_row_at_index(messageList, index);
	printf("ROW ::: M: %p\tT: %p\tBOOL: %d\n", (void *)message->row, (void *)temp, GTK_WIDGET(temp) == message->row);
	if (message && message->row) {
		// gtk_container_remove(GTK_CONTAINER(messageList), message->row);
		gtk_widget_destroy(message->row);
		message->row = NULL;
	}

}


gboolean mx_edit_message(t_edit_data *edit){
	GtkWidget *temp_widget = NULL;

	if (edit->message && edit->message->row && edit->new_message){
		// gtk_container_remove(GTK_CONTAINER(messageList), edit->message->row);
		// temp_widget = gtk_list_box_get_row_at_index(messageList, edit->index);

    	mx_delete_message_row(edit->message, edit->index);
		// gtk_widget_destroy(edit->message->row);
		// gtk_container_add(GTK_CONTAINER(edit->new_message->row), NULL);
		edit->message->row = mx_create_out_mess(edit->new_message->data, edit->new_message->nickname);

		printf("%d\n", edit->index);
		gtk_widget_show(edit->message->row);
		gtk_list_box_insert(messageList, edit->message->row, edit->index);

		gtk_entry_set_text(GTK_ENTRY(sendEntry), "");
		// gtk_widget_show_all(temp_widget);
		g_signal_handler_disconnect(sendEntry, hendler_id_entry);
		g_signal_handler_disconnect(sendButton, hendler_id_button);
		hendler_id_entry = g_signal_connect(G_OBJECT(sendEntry),"activate", G_CALLBACK(do_send), NULL);
		hendler_id_button = g_signal_connect(G_OBJECT(sendButton),"clicked", G_CALLBACK(do_send), NULL);
	}
	return FALSE;
}


	// printf("TEMP :%s\n", temp->data);
	// mx_do_message_request(temp, "update_message");

gboolean mx_delete_message(t_user_message *new_message) {
	t_user_message *message = NULL;
    // printf("LOOKING FOR INDEX\t:::\t\n");
	int index_msg = mx_get_index_history_message(history_message_list, new_message->owner_id, new_message->tv_id, &message);

    // printf("INDEX\t\t:::\t%d\n", index_msg);
    if (message && index_msg != -1) {
    	// printf("POPING INDEX\t\t:::\t\n");
    	mx_pop_index(history_message_list, index_msg);
    	// printf("DELETE ROW\t\t:::\t\n");
    	mx_delete_message_row(message, index_msg);
    	// printf("DELETE MESSAGE\t\t:::\t\n");
    	mx_messdel(&message);
    	// printf("DELETE DONE\t\t:::\t\n");
    }
    return FALSE;
	// for (t_list_node *w = history_message_list->head; w != NULL; w = w->next) {
 //    		int u_id = ((t_user_message *)(w->data))->owner_id;
 //    	    time_t u_time =((t_user_message *)(w->data))->tv_id;
    		
 //    		if ( u_id == new_message->owner_id && u_time == new_message->tv_id) {
 //    			t_list_node *temp = w;

 //    			if (history_message_list->head != history_message_list->tail) {
 //    				if (w == history_message_list->head) {
	// 					history_message_list->head = w->next;
	// 					w->next->prev = NULL;
	//     			} 
	//     			else if (w == history_message_list->tail) {
	//     				history_message_list->tail = w->prev;
	//     				w->prev->next = NULL;
	//     			} 
	// 	    		else {
	// 					w->prev->next = w->next;
	//     			}
 //    			} 
 //    			else {
 //    				history_message_list->head = NULL;
 //    				history_message_list->tail = NULL;
 //    			}
	   			
 //    			history_message_list->size--;
 //    			mx_delete_message_row((t_user_message *)(temp->data));
 //    			mx_messdel((t_user_message **)(&(temp->data)));
	// 			free(temp);
 //    			tls_write(0, "NODE DELETED\n", strlen("NODE DELETED\n"));
 //    			break;
 //    		}
 //    	}
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
    	// mx_push_back(history_message_list, new_message);
    	edit->message = message;
    	edit->new_message = new_message;
    	edit->index = index_msg;
    	gdk_threads_add_idle((GSourceFunc)mx_edit_message, edit);
    	// mx_edit_message(message, new_message, index_msg);
    }
    else if (strcmp(type, "delete_message_back") == 0) {
    	// printf("START DELETE\t:::\t\n");
    	// mx_push_back(history_message_list, new_message);
    	gdk_threads_add_idle((GSourceFunc)mx_delete_message, new_message);
    	// mx_delete_message(new_message);
    	//index_msg = mx_get_index_history_message(history_message_list, new_message->owner_id, new_message->tv_id, &message);
    	// if (message && index_msg != -1) {
    	// 	mx_pop_index(history_message_list, index_msg);
    	// 	mx_messdel(&message);
    	// }
    }
}

int mx_do_reconnection(int rc) {
	char buffer[BUF_SIZE];
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
		if (init_connection(0, info->argv, info->socket) == 0)
			return 0;
		if (counter++ == CNCT_AM)
			return -1;
	}
	return 0;
}

void *read_server_thread(void *par) {
	char answ[BUF_SIZE];
	int tail = 0;
	t_user_message *message = NULL;

	while (true) {
		if ((tail = tls_read(tls_ctx, answ, BUF_SIZE)) <= 0) {
			tail = mx_do_reconnection(tail);
			if (tail < 0)
				exit(0);
			// mx_clear_screen();
			message_request_history();
			continue;
		}
		answ[tail] = '\0';
		printf("SERVER THREAD: %s\n", answ);
		message = mx_proc_server_back(answ, &owner);
		if (!message)
			continue;
		mx_switch_message_back(&owner, message);
		message = NULL;
		// print_history(history_message_list);
	}
	return par;
}

void do_send() {
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
	pthread_create(&watcher, 0, watcher_thread, (void *)message);
	gtk_entry_set_text(GTK_ENTRY(sendEntry), "");
}

static void zero_string(char *arr) {
	for (int i =0; i < BUF_SIZE; i++)
		arr[i] = '\0';
}

int mx_history_size(json_object *jobj) {
	const char *type = json_object_get_string(json_object_object_get(jobj, "type"));

	if (strcmp(type, "hst_size") == 0) {
		printf("JSON SIZE ::: %d\n", json_object_get_int(json_object_object_get(jobj, "size")));
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
		printf("JSON MESSAGE ::: \t%s\t%s\n", type, (message->data));
		json_object_put(jobj);
		// zero_string(buffer);

	}
	return history_message_list;
}

/* ============== TEST HISTORY ================*/

void static print_node(t_list_node *node) {
	static int i = 0;
	t_user_message *mess = (t_user_message *)(node->data);

	printf("\n=================================================================================================\n");
	printf("%d)\tMESSAGE FROM \"%s\"\t#%lu\tuser id: %d\t:::\t%s\n", i, mess->nickname, mess->tv_id, mess->owner_id, mess->data);
	printf("=================================================================================================\n");
	i++;
}

void static print_history(t_list *list) {
	system("clear");
	printf("\n\t\t\t::: HISTORY LIST :::\n");
	mx_foreach_list(list, print_node);
}


/* ============== TEST HISTORY END ==============*/

void mx_do_history_request() {
	const char *data;
	json_object *jobj = json_object_new_object();
	json_object *j_type = json_object_new_string("history_request");
	json_object_object_add(jobj, "type", j_type);

	data = json_object_to_json_string(jobj);
	tls_write(tls_ctx, data, strlen(data));
	// mx_strdel((char **)&data);
}

void message_request_history(void) {
	char *data = NULL;
	char answ[BUF_SIZE];
	json_object *jobj;
	t_client_info *clnt = get_client_info();

	// printf("*\n");
	mx_do_history_request();
	// printf("**\n");
	zero_string(answ); /////////// не обнулять а через рид
	// printf("***\n");
	tls_read(tls_ctx, answ, BUF_SIZE);
	if (strcmp("no_history", answ) == 0)
		return;
	// printf("****\n");
	jobj = json_tokener_parse(answ);
	// printf("*****\n");
	int hst_size = mx_history_size(jobj);
	// printf("******\n");
	mx_create_hst_message_list(hst_size);
	// printf("*******\n");
	json_object_put(jobj);
}

void init_chat_window(char *nickname)
{
	GtkBuilder *builder = gtk_builder_new_from_resource("/org/gtk/client/chat.glade");
	GtkWidget *box1;
	gint h, w;
	history_message_list = mx_create_list();
	chatWindow = GTK_WIDGET(gtk_builder_get_object(builder,"ChatWindow"));
	char buf[100] = "uchat : ";
	strcat(buf, nickname);
	gtk_window_set_title(GTK_WINDOW(chatWindow), buf);
	g_signal_connect(chatWindow,"destroy", G_CALLBACK(gtk_main_quit),NULL);

	gtk_window_set_position(GTK_WINDOW(chatWindow), GTK_WIN_POS_CENTER);
	sendEntry = GTK_WIDGET(gtk_builder_get_object(builder,"SendEntry"));
	sendButton = GTK_WIDGET(gtk_builder_get_object(builder,"SendButton"));
	hendler_id_entry = g_signal_connect(G_OBJECT(sendEntry),"activate", G_CALLBACK(do_send),NULL);
	hendler_id_button = g_signal_connect(G_OBJECT(sendButton),"clicked", G_CALLBACK(do_send),NULL);
	//    statusLabel = GTK_WIDGET(gtk_builder_get_object(builder,"statusLabel"));
	scrolledWindow = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder,"ScrolledWindow"));
	vAdjust = gtk_scrolled_window_get_vadjustment(scrolledWindow);
	messageList = GTK_LIST_BOX(gtk_builder_get_object(builder,"MessageListBox"));
	
	message_request_history();
	print_history(history_message_list);



	/**************** TEST ***************/

	// int o_id = 3;
	// int m_id = 1594995841;
	// t_user_message *message = NULL;
	// int indx = mx_get_index_history_message(history_message_list, o_id, m_id, &message);
	// printf("INDEX ::: #%d\t:::\t\" %p \"\n", indx, message);

	/************* TEST END **************/
	// box1 = mx_create_out_mess("dwdw", "dwdwd");
	GtkCssProvider *cssStyle;
	cssStyle = gtk_css_provider_new();
	gtk_css_provider_load_from_path(cssStyle, "./client/src/style.css", NULL);
	mx_css_set(cssStyle, chatWindow);
	mx_css_set(cssStyle, sendButton);
	mx_css_set(cssStyle, sendEntry);
	// gtk_list_box_insert (messageList, box1, -1);
	pthread_create(&server, 0, read_server_thread, NULL);

	
	/**************************************************/

	//  owner.last_server_back = "delete_message_back";
	// t_user_message *m = (t_user_message *)malloc(sizeof(t_user_message));

 //    m->owner_id = 1;
 //    m->data = "test";
 //    m->tv_id = time(NULL);
 //    m->nickname = "Vasya";

	// mx_switch_message_back(&owner, m);

	 /*************************************************/


	/************* TEST EDIT *************/

	// for (int i = 0; i < 2000000000; i++);
	// t_user_message *message = history_message_list->head->next->next->data;

	// t_user_message *temp = mx_create_edit_message(message, "SPY");
	// printf("TEMP :%s\n", temp->data);
	// mx_do_message_request(temp, "update_message");
	// // print_history(history_message_list);
	// printf("<<<<<<<<<<< %s >>>>>>>>>>>>\n", (char *)(((t_user_message *)history_message_list->head->next->next->data)->data));

	/*********** TEST EDIT END ***********/

	/************ TEST DELETE ************/

	//for (int i = 0; i < 2000000000; i++);
	// sleep(1);
	// //t_user_message *message = history_message_list->head->next->next->data;
	// //t_user_message *message = history_message_list->head->next->data;
	// t_user_message *message = NULL;
	// if (history_message_list->head) {
	// 	message = history_message_list->head->data;
	// }
	

	// // t_user_message *temp = mx_create_edit_message(message, "HELLO, WORLD!");
	// // printf("TEMP :%s\n", temp->data);
	// mx_do_message_request(message, "delete_message");
	// print_history(history_message_list);
	// printf("<<<<<<<<<<< %s >>>>>>>>>>>>\n", (char *)(((t_user_message *)history_message_list->head->next->next->data)->data));

	/********** TEST DELETE END **********/
}

// void init_chat_window(char *nickname)
// {
// 	GtkBuilder *builder = gtk_builder_new_from_resource("/org/gtk/client/chat.glade");

// 	history_message_list = mx_create_list();
// 	chatWindow = GTK_WIDGET(gtk_builder_get_object(builder,"chatWindow"));
// 	char buf[100] = "uchat : ";
// 	strcat(buf, nickname);
// 	gtk_window_set_title(GTK_WINDOW(chatWindow), buf);
// 	g_signal_connect(chatWindow,"destroy", G_CALLBACK(gtk_main_quit),NULL);

	
// 	gtk_window_set_position(GTK_WINDOW(chatWindow), GTK_WIN_POS_CENTER);
// 	sendEntry = GTK_WIDGET(gtk_builder_get_object(builder,"sendEntry"));
// 	sendButton = GTK_WIDGET(gtk_builder_get_object(builder,"sendButton"));
// 	g_signal_connect(G_OBJECT(sendEntry),"activate", G_CALLBACK(do_send),NULL);
// 	g_signal_connect(G_OBJECT(sendButton),"clicked", G_CALLBACK(do_send),NULL);
// //    statusLabel = GTK_WIDGET(gtk_builder_get_object(builder,"statusLabel"));
// //    messagesTreeView = GTK_WIDGET(gtk_builder_get_object(builder,"messagesTreeView"));
// //    messagesListStore = GTK_LIST_STORE(gtk_builder_get_object(builder,"messagesListStore"));
// 	scrolledWindow = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder,"scrolledWindow"));
// 	vAdjust = gtk_scrolled_window_get_vadjustment(scrolledWindow);
// 	message_request_history();
// 	print_history(history_message_list);
// 	// mx_do_history_request(3);
// 	pthread_create(&server, 0, read_server_thread, 0);
// }
