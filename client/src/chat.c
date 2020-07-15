#include "../inc/client.h"

#define REQUEST_HISTORY 10

GtkWidget *chatWindow;
GtkWidget *sendEntry, *sendButton;
GtkWidget *statusLabel;
GtkWidget *messagesTreeView;
GtkAdjustment *vAdjust;
GtkScrolledWindow *scrolledWindow;
GtkListStore *messagesListStore;
pthread_t watcher;

// void *watcher_thread(void *param) {
// 	(void) param;
// 	scrolledWindow = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder,"scrolledWindow"));
// 	vAdjust = gtk_scrolled_window_get_vadjustment(scrolledWindow);
// 	pthread_create(&watcher, 0, watcher_thread, 0);
// }

void *watcher_thread(void *param)
{
    (void) param;
    struct timeval tv;
    struct tm *nowtm;
    char *author = NULL, *body = NULL;
    char timebuf[64];
    // message_request_history(REQUEST_HISTORY);
    while(1)
    {
        // int k = message_receive(&tv, &author, &body);
        // if(k < 0)
        // {
        //     gtk_label_set_text(GTK_LABEL(statusLabel), "Disconnected, please restart the client");
        //     gtk_widget_set_sensitive(sendButton, 0);
        //     break;
        // }
        // if(k == 0)
        //     continue;
        if(!author)
        {
            gtk_label_set_text(GTK_LABEL(statusLabel), body);
            continue;
        }
        if(tv.tv_sec)
        {
            nowtm = localtime(&tv.tv_sec);
            strftime(timebuf, 64, "[%d.%m.%Y %H:%M:%S]", nowtm);
        }
        else
        {
            *timebuf = 0;
        }
        // add_list_entry(timebuf, author, body, k != 'h');
        free(author);
        free(body);
    }
    return param;
}

void init_chat_window(char *nickname)
{
    GtkBuilder *builder = gtk_builder_new_from_resource("/org/gtk/client/chat.glade");

    chatWindow = GTK_WIDGET(gtk_builder_get_object(builder,"chatWindow"));
    char buf[100] = "uchat : ";
    strcat(buf, nickname);
    gtk_window_set_title(GTK_WINDOW(chatWindow), buf);
    g_signal_connect(chatWindow,"destroy", G_CALLBACK(gtk_main_quit),NULL);
    sendEntry = GTK_WIDGET(gtk_builder_get_object(builder,"SendEntry"));
    sendButton = GTK_WIDGET(gtk_builder_get_object(builder,"SendButton"));
//    g_signal_connect(G_OBJECT(sendEntry),"activate", G_CALLBACK(do_send),NULL);
//    g_signal_connect(G_OBJECT(sendButton),"clicked", G_CALLBACK(do_send),NULL);
//    statusLabel = GTK_WIDGET(gtk_builder_get_object(builder,"statusLabel"));
//    messagesTreeView = GTK_WIDGET(gtk_builder_get_object(builder,"messagesTreeView"));
//    messagesListStore = GTK_LIST_STORE(gtk_builder_get_object(builder,"messagesListStore"));
    scrolledWindow = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder,"scrolledWindow"));
    vAdjust = gtk_scrolled_window_get_vadjustment(scrolledWindow);
    // pthread_create(&watcher, 0, watcher_thread, 0);
}
