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


// void *watcher_thread(void *param) {
// 	(void) param;
// 	scrolledWindow = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder,"scrolledWindow"));
// 	vAdjust = gtk_scrolled_window_get_vadjustment(scrolledWindow);
// 	pthread_create(&watcher, 0, watcher_thread, 0);
// }


int show_popup(GtkWidget *widget, GdkEvent *event) {

	const gint RIGHT_CLICK = 3;

	if (event->type == GDK_BUTTON_PRESS) {

		GdkEvent *bevent = (GdkEvent *) event;

		gtk_menu_popup_at_pointer(GTK_MENU(widget), bevent);

		return TRUE;
	}

	return FALSE;
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

	GtkCssProvider *cssStyle2;
	GtkCssProvider *cssStyle3;
	cssStyle2 = gtk_css_provider_new();
	cssStyle3 = gtk_css_provider_new();

	gtk_css_provider_load_from_path(cssStyle2, "./client/src/input_mess_style.css", NULL);
	gtk_css_provider_load_from_path(cssStyle3, "./client/src/other_mess_style.css", NULL);

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
	G_CALLBACK(show_popup), popup_menu);

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

	mx_css_set(cssStyle3, message);
	// mx_css_set(cssStyle2, edit);
	mx_css_set(cssStyle3, delete);
	mx_css_set(cssStyle3, popup_menu);
	mx_css_set(cssStyle3, nickname);
	mx_css_set(cssStyle3, tool_box);
	mx_css_set(cssStyle3, time);
	mx_css_set(cssStyle3, message_label);

	return row;
}

GtkWidget *mx_create_out_mess(const char *message_text, const char *login_text){
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
	GtkWidget *edit;
	GtkWidget *delete;
	GtkWidget *ebox;

	tool_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, TRUE);
	edit = gtk_button_new();
	delete = gtk_button_new();
	gtk_container_add (GTK_CONTAINER(tool_box), edit);
	gtk_container_add (GTK_CONTAINER(tool_box), delete);

	popup_menu = gtk_menu_new();

	GtkCssProvider *cssStyle2;
	GtkCssProvider *cssStyle3;
	cssStyle2 = gtk_css_provider_new();
	cssStyle3 = gtk_css_provider_new();

	gtk_css_provider_load_from_path(cssStyle2, "./client/src/input_mess_style.css", NULL);
	gtk_css_provider_load_from_path(cssStyle3, "./client/src/other_mess_style.css", NULL);

	row = gtk_list_box_row_new ();

	gtk_list_box_row_set_activatable(GTK_LIST_BOX_ROW(row), FALSE); // !
	gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(row), FALSE); // !

	in_row = gtk_box_new (GTK_ORIENTATION_VERTICAL, TRUE);
	nickname = gtk_label_new(login_text);
	gtk_widget_set_halign (nickname, GTK_ALIGN_END);
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

	gtk_container_add (GTK_CONTAINER(container), void_box);
	gtk_container_add (GTK_CONTAINER(container), in_row);

	edit = gtk_menu_item_new_with_label("Edit");
	gtk_widget_show(edit);
	gtk_menu_shell_append(GTK_MENU_SHELL(popup_menu), edit);

	delete = gtk_menu_item_new_with_label("Delete");
	gtk_widget_show(delete);
	gtk_menu_shell_append(GTK_MENU_SHELL(popup_menu), delete);

	g_signal_connect_swapped(G_OBJECT(ebox), "button-press-event",
	G_CALLBACK(show_popup), popup_menu);

	gtk_container_add (GTK_CONTAINER(row), container);
	gtk_widget_set_halign (row, GTK_ALIGN_END);

	gtk_widget_set_name(message, "row_to");
	gtk_widget_set_name(edit, "edit");
	gtk_widget_set_name(delete, "edit");
	gtk_widget_set_name(popup_menu, "popup_menu");
	gtk_widget_set_name(nickname, "nickname");
	gtk_widget_set_name(time, "time");
	gtk_widget_set_name(tool_box, "popup_menu");
	gtk_widget_set_name(message_label, "message");

	mx_css_set(cssStyle2, message);
	mx_css_set(cssStyle2, edit);
	mx_css_set(cssStyle2, delete);
	mx_css_set(cssStyle2, popup_menu);
	mx_css_set(cssStyle2, nickname);
	mx_css_set(cssStyle2, tool_box);
	mx_css_set(cssStyle2, time);
	mx_css_set(cssStyle2, message_label);

	return row;
}

void static print_history(t_list *history_message_list);

void mx_messdel(t_user_message **message) {
	if (message && *message) {
		mx_strdel((char **)&((*message)->nickname));
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
    char *user_nickname = mx_strdup((char *)json_object_get_string(json_object_object_get(jobj, "user_nickname")));
    int time_id = json_object_get_int(json_object_object_get(jobj, "msg_time"));
    char *msg_body = mx_strdup((char *)json_object_get_string(json_object_object_get(jobj, "msg_body")));
    t_user_message *message = NULL;

    if (error == 0) {
    	message = (t_user_message *)malloc(sizeof(t_user_message));
	    printf("\nMESSAGE from %s\t:::\t\" %s \"\n", user_nickname, msg_body);
	    message->owner_id = user_id;
	    message->data = msg_body;
	    message->tv_id = time_id;
	    message->nickname = user_nickname;
	}
	if (message) {
		printf("DEBUG_MESSAGE: %d, %s, %lu, %s\n", message->owner_id, message->data, message->tv_id, message->nickname);
	}
    return message;
}

void mx_do_message_request(t_user_message *message, const char *request) {
	t_client_info *clnt = get_client_info();
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
	if (write(clnt->sock, data, strlen(data)) == -1) {
		printf("error = %s\n", strerror(errno));
	}
	// read(clnt->sock, answ, BUF_SIZE);

	// mx_printstr("SERVER: ", 0);													/***************/
	// mx_printstr(answ, 0);
	// mx_printstr("\n", 0);
	json_object_put(jobj);
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

void mx_add_message_widget(t_user_message *new_message) {
	GtkWidget *temp = NULL;

	if (!new_message)
		return;
	if(new_message->owner_id == owner.id){
    		temp = mx_create_out_mess(new_message->data, new_message->nickname);
    	}
    	else {
    		temp = mx_create_in_mess(new_message->data, new_message->nickname);
    	}
    	gtk_widget_show_all(temp);
    	gtk_list_box_insert(messageList, temp, -1);
}

void mx_switch_message_back(t_user_info *user, t_user_message *new_message) {
	char *type = user->last_server_back;
	t_user_message *message = NULL;
	int index_msg = 0;

    if (strcmp(type, "new_message_back") == 0) {
    	mx_push_back(history_message_list, new_message);
    	mx_add_message_widget(new_message);
    	
    }
    else if (strcmp(type, "update_message_back") == 0) {
    	index_msg = mx_get_index_history_message(history_message_list, new_message->owner_id, new_message->tv_id, &message);
    	message->data = new_message->data;
    }
    else if (strcmp(type, "delete_message_back") == 0) {
    	//index_msg = mx_get_index_history_message(history_message_list, new_message->owner_id, new_message->tv_id, &message);
    	// if (message && index_msg != -1) {
    	// 	mx_pop_index(history_message_list, index_msg);
    	// 	mx_messdel(&message);
    	// }

    	for (t_list_node *w = history_message_list->head; w != NULL; w = w->next) {
    		int u_id = ((t_user_message *)(w->data))->owner_id;
    	    time_t u_time =((t_user_message *)(w->data))->tv_id;

    		// printf("\nNODE\t:::\t\t%d, %lu\n", u_id, u_time);
    		// printf("FROM SERVER MSG\t:::\t%d, %lu\n", new_message->owner_id, new_message->tv_id);
    		
    		if ( u_id == new_message->owner_id && u_time == new_message->tv_id) {
    			t_list_node *temp = w;

    			if (history_message_list->head != history_message_list->tail) {
    				if (w == history_message_list->head) {
						history_message_list->head = w->next;
						w->next->prev = NULL;
	    			} 
	    			else if (w == history_message_list->tail) {
	    				history_message_list->tail = w->prev;
	    				w->prev->next = NULL;
	    			} 
		    		else {
						w->prev->next = w->next;
	    			}
    			} 
    			else {
    				history_message_list->head = NULL;
    				history_message_list->tail = NULL;
    			}
	   			
    			history_message_list->size--;
    			
    			mx_messdel((t_user_message **)(&(temp->data)));
				free(temp);
    			write(0, "NODE DELETED\n", strlen("NODE DELETED\n"));
    			break;
    		}
    	}
    }
}

void *read_server_thread(void *par) {
	t_client_info *clnt = get_client_info();
	char *data = NULL;
	char answ[BUF_SIZE];
	bool flag = true;
	int tail = 0;
	t_user_message *message;

	while (flag) {
		if ((tail = read(clnt->sock, answ, BUF_SIZE)) == -1) {
			printf("error = %s\n", strerror(errno));
			flag = false;
		}
		answ[tail] = '\0';
		mx_printstr("SERVER THREAD: ", 0);
		mx_printstr(answ, 0);
		mx_printstr("\n", 0);
		message = mx_proc_server_back(answ, &owner);
		if (!message)
			continue;
		mx_switch_message_back(&owner, message);
		print_history(history_message_list);
	}
	return par;
}

void do_send() {
	const gchar *mess_data;

	mess_data = strdup(gtk_entry_get_text(GTK_ENTRY(sendEntry)));
	if(!mess_data || !*mess_data)
	{
		gtk_widget_grab_focus(sendEntry);
		return;
	}

	t_user_message *message = (t_user_message *)malloc(sizeof(t_user_message)); // memleak possible
	message->owner_id = owner.id;
	message->data = (char *)mess_data;
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

void mx_do_history_ready(int fd) {						//*
	write(fd, "r", 1);									//	*	READY
}														//*

t_list *mx_create_hst_message_list(int list_size, int fd) {
	char buffer[BUF_SIZE];
	json_object *jobj;
	const char *type = NULL;
	t_user_message *message = NULL;

	for (int i = 0; i < list_size; ++i, message = NULL) {
		mx_do_history_ready(fd);
		read(fd, buffer, BUF_SIZE);
		jobj = json_tokener_parse(buffer);

		type = json_object_get_string(json_object_object_get(jobj, "type"));
		if (strcmp(type, "hst_list") == 0) {
			message = mx_proc_message_back(jobj);
			mx_push_back(history_message_list, message);
			mx_add_message_widget(message);
		}
		printf("JSON MESSAGE ::: \t%s\t%s\n", type, (mx_proc_message_back(jobj))->data);
		json_object_put(jobj);
		zero_string(buffer);

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

void mx_do_history_request(int fd) {
	const char *data;
	json_object *jobj = json_object_new_object();
	json_object *j_type = json_object_new_string("history_request");
	json_object_object_add(jobj, "type", j_type);

	data = json_object_to_json_string(jobj);
	write(fd, data, strlen(data));
	// mx_strdel((char **)&data);
}

void message_request_history(void) {
	t_client_info *clnt = get_client_info();
	char *data = NULL;
	char answ[BUF_SIZE];
	json_object *jobj;

	// printf("*\n");
	mx_do_history_request(clnt->sock);
	// printf("**\n");
	zero_string(answ); /////////// не обнулять а через рид
	// printf("***\n");
	read(clnt->sock, answ, BUF_SIZE);
	if (strcmp("no_history", answ) == 0)
		return;
	// printf("****\n");
	jobj = json_tokener_parse(answ);
	// printf("*****\n");
	int hst_size = mx_history_size(jobj);
	// printf("******\n");
	mx_create_hst_message_list(hst_size, clnt->sock);
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
	g_signal_connect(G_OBJECT(sendEntry),"activate", G_CALLBACK(do_send),NULL);
	g_signal_connect(G_OBJECT(sendButton),"clicked", G_CALLBACK(do_send),NULL);
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
	box1 = mx_create_out_mess("dwdw", "dwdwd");
	GtkCssProvider *cssStyle;
	cssStyle = gtk_css_provider_new();
	gtk_css_provider_load_from_path(cssStyle, "./client/src/style.css", NULL);
	mx_css_set(cssStyle, chatWindow);
	mx_css_set(cssStyle, sendButton);
	mx_css_set(cssStyle, sendEntry);
	// gtk_list_box_insert (messageList, box1, -1);
	pthread_create(&server, 0, read_server_thread, 0);

	
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
