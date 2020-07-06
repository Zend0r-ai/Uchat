#include "chat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

GtkWidget *chatWindow;
GtkWidget *sendEntry, *sendButton;
GtkWidget *statusLabel;
GtkWidget *messagesTreeView;
GtkAdjustment *vAdjust;
GtkScrolledWindow *scrolledWindow;
GtkListStore *messagesListStore;
pthread_t watcher;

void init_chat_window(char *login)
{
    GtkBuilder *builder = gtk_builder_new_from_resource("/org/gtk/client/chat.glade");

    chatWindow = GTK_WIDGET(gtk_builder_get_object(builder,"ChatWindow"));
//    char buf[100] = "AKOS chat client: ";
//    strcat(buf, login);
    gtk_window_set_title(GTK_WINDOW(chatWindow), buf);
//    g_signal_connect(chatWindow,"destroy", G_CALLBACK(gtk_main_quit),NULL);
    sendEntry = GTK_WIDGET(gtk_builder_get_object(builder,"SendEntry"));
    sendButton = GTK_WIDGET(gtk_builder_get_object(builder,"SendButton"));
//    g_signal_connect(G_OBJECT(sendEntry),"activate", G_CALLBACK(do_send),NULL);
//    g_signal_connect(G_OBJECT(sendButton),"clicked", G_CALLBACK(do_send),NULL);
//    statusLabel = GTK_WIDGET(gtk_builder_get_object(builder,"statusLabel"));
//    messagesTreeView = GTK_WIDGET(gtk_builder_get_object(builder,"messagesTreeView"));
//    messagesListStore = GTK_LIST_STORE(gtk_builder_get_object(builder,"messagesListStore"));
    scrolledWindow = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder,"ScrolledWindow"));
    vAdjust = gtk_scrolled_window_get_vadjustment(scrolledWindow);
    pthread_create(&watcher, 0, watcher_thread, 0);
}
