//LoginLabel

#include "../inc/client.h"

GtkWidget *loginEntry, *passwordEntry;
GtkWidget *regButton;
GtkWidget *signUpWindow;
pthread_t reginer;
int regged_in;

struct reg_info
{
    const char *login;
    const char *password;
};

// void *login_thread(void *param)
// {
// 	// char *res = "incorrect login or password";
//     char *res = NULL;
// //    char *res = message_connect(((struct reg_info *)param)->ip, ((struct reg_info *)param)->iport);
//     //ippppp port
// //	if(!res)
// //        res = message_do_login(((struct reg_info *)param)->login, ((struct reg_info *)param)->password);
//     if(res)
//     {
//         gtk_label_set_text(GTK_LABEL(statusLabel), res);
//         // message_disconnect();
//     }
//     else
//     {
//         init_chat_window(((struct reg_info *)param)->login);
//         regged_in = 1;
//         free(param);
//         return param;
//     }
//     free(param);
//     gtk_widget_set_sensitive(reg_button, 1);
//     return param;
// }

void do_reg(GtkWidget *widget, gpointer data)
{
    (void) widget;
    (void) data;
    if(!gtk_widget_get_sensitive(regButton))
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
    gtk_widget_set_sensitive(regButton, 0);
    struct reg_info *ri = malloc(sizeof(struct reg_info));
    ri->login = login;
    ri->password = password;
    gtk_widget_hide(signUpWindow);
    gtk_widget_show_all(StartWindow);
    // pthread_create(&reginer, 0, login_thread, (void *)ri);
}

void init_sign_up_window()
{
    GtkBuilder *builder = gtk_builder_new_from_resource("/org/gtk/client/sign_up.glade");

    signUpWindow = GTK_WIDGET(gtk_builder_get_object(builder,"reg_window"));
    g_signal_connect(signUpWindow,"destroy", G_CALLBACK(init_start_window),NULL);

    loginEntry = GTK_WIDGET(gtk_builder_get_object(builder,"reg_login_entry"));
    passwordEntry = GTK_WIDGET(gtk_builder_get_object(builder,"reg_pass_entry"));
    g_signal_connect(G_OBJECT(loginEntry),"activate", G_CALLBACK(do_reg),NULL);
    g_signal_connect(G_OBJECT(passwordEntry),"activate", G_CALLBACK(do_reg),NULL);
    regButton = GTK_WIDGET(gtk_builder_get_object(builder,"reg_button"));
    g_signal_connect(G_OBJECT(regButton),"clicked", G_CALLBACK(do_reg),NULL);
    regged_in = 0;
    // g_timeout_add(50, check_login, 0);
}
