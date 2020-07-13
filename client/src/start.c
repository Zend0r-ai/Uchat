//LoginLabel

#include "../inc/client.h"


GtkWidget *RegButton;
GtkWidget *SignInButton;
GtkWidget *StartWindow;

pthread_t loginner;

// void *login_thread(void *param)
// {
//     init_login_window();
//     free(param);
//     gtk_widget_set_sensitive(SignInButton, 1);
//     return param;
// }

// void *reg_thread(void *param)
// {
//     init_login_window();
//     free(param);
//     gtk_widget_set_sensitive(SignInButton, 1);
//     return param;
// }

void open_login_win(GtkWidget *widget, gpointer data)
{
    (void) widget;
    (void) data;
    init_login_window();
    gtk_widget_set_sensitive(SignInButton, 1);
    gtk_widget_hide(StartWindow);
    gtk_widget_show_all(loginWindow);
    // pthread_create(&loginner, 0, login_thread);
}

void open_reg_win(GtkWidget *widget, gpointer data)
{
    (void) widget;
    (void) data;
    init_sign_up_window();
    gtk_widget_set_sensitive(RegButton, 1);
    gtk_widget_hide(StartWindow);
    gtk_widget_show_all(signUpWindow);
    // pthread_create(&loginner, 0, reg_thread);
}

void init_start_window()
{
    GtkBuilder *builder = gtk_builder_new_from_resource("/org/gtk/client/start.glade");

    StartWindow = GTK_WIDGET(gtk_builder_get_object(builder,"start_window"));
    g_signal_connect(StartWindow,"destroy", G_CALLBACK(gtk_main_quit),NULL);

    RegButton = GTK_WIDGET(gtk_builder_get_object(builder,"OpenSignUpWin"));
    g_signal_connect(G_OBJECT(RegButton),"clicked", G_CALLBACK(open_reg_win),NULL);
    SignInButton = GTK_WIDGET(gtk_builder_get_object(builder,"OpenSignInWin"));
    g_signal_connect(G_OBJECT(SignInButton),"clicked", G_CALLBACK(open_login_win),NULL);
}
