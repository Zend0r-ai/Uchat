//LoginLabel

#include "login.h"
#include "chat.h"
#include <stdio.h>
#include <stdlib.h>

GtkWidget *loginEntry, *passwordEntry;
GtkWidget *statusLabel;
GtkWidget *loginButton;
GtkWidget *loginWindow;
pthread_t loginner;
int logged_in;

struct login_info
{
    const char *login;
    const char *password;
};

void *login_thread(void *param)
{
	// char *res = "incorrect login or password";
    char *res = NULL;
//    char *res = message_connect(((struct login_info *)param)->ip, ((struct login_info *)param)->iport);
    //ippppp port
//	if(!res)
//        res = message_do_login(((struct login_info *)param)->login, ((struct login_info *)param)->password);
    if(res)
    {
        gtk_label_set_text(GTK_LABEL(statusLabel), res);
        // message_disconnect();
    }
    else
    {
        init_chat_window(((struct login_info *)param)->login);
        logged_in = 1;
        free(param);
        return param;
    }
    free(param);
    gtk_widget_set_sensitive(loginButton, 1);
    return param;
}

void do_login(GtkWidget *widget, gpointer data)
{
    (void) widget;
    (void) data;
    if(!gtk_widget_get_sensitive(loginButton))
        return;
    const gchar *login, *password;
    login = gtk_entry_get_text(GTK_ENTRY(loginEntry));
    if(!login || !*login)
    {
        gtk_widget_grab_focus(loginEntry);
        return;
    }
    password = gtk_entry_get_text(GTK_ENTRY(passwordEntry));
    if(!password || !*password)
    {
        gtk_widget_grab_focus(passwordEntry);
        return;
    }
    gtk_widget_set_sensitive(loginButton, 0);
    struct login_info *li = malloc(sizeof(struct login_info));
    li->login = login;
    li->password = password;
    pthread_create(&loginner, 0, login_thread, (void *)li);
}


gboolean check_login(void *param)
{
    (void) param;
    if(logged_in)
    {
        gtk_widget_hide(loginWindow);
        gtk_widget_show_all(chatWindow);
        logged_in = 0;
        return G_SOURCE_REMOVE;
    }
    return G_SOURCE_CONTINUE;
}

void init_login_window()
{
    GtkBuilder *builder = gtk_builder_new_from_resource("/org/gtk/client/login.glade");

    loginWindow = GTK_WIDGET(gtk_builder_get_object(builder,"LoginWin"));
    g_signal_connect(loginWindow,"destroy", G_CALLBACK(gtk_main_quit),NULL);


    loginEntry = GTK_WIDGET(gtk_builder_get_object(builder,"LoginEntry"));
    passwordEntry = GTK_WIDGET(gtk_builder_get_object(builder,"PassEntry"));
    g_signal_connect(G_OBJECT(loginEntry),"activate", G_CALLBACK(do_login),NULL);
    g_signal_connect(G_OBJECT(passwordEntry),"activate", G_CALLBACK(do_login),NULL);
    statusLabel = GTK_WIDGET(gtk_builder_get_object(builder,"StatusLabel"));
    loginButton = GTK_WIDGET(gtk_builder_get_object(builder,"LoginButton"));
    g_signal_connect(G_OBJECT(loginButton),"clicked", G_CALLBACK(do_login),NULL);
    logged_in = 0;
    g_timeout_add(50, check_login, 0);
}
