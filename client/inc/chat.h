#ifndef CHAT_H
#define CHAT_H

#include "client.h"

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

#endif
