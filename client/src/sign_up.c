//LoginLabel

#include "../inc/client.h"

GtkWidget *loginEntry, *passwordEntry;
GtkWidget *regButton;
GtkWidget *signUpWindow;
GtkWidget *statusLabel;
pthread_t reginer;
int regged_in;

struct reg_info
{
    char *login;
    char *password;
    char *nickname;
};

char *message_do_sing_up(struct reg_info *reg_par) {
    t_client_info *clnt = get_client_info();
    char *data = NULL;

                reg_par->nickname = "ogur4ik";


    json_object *jobj = json_object_new_object();
    json_object *j_type = json_object_new_string("sing_up");
    json_object *j_login = json_object_new_string(reg_par->login);
    json_object *j_passwd = json_object_new_string(reg_par->password);
    json_object *j_nick = json_object_new_string(reg_par->nickname);

    json_object_object_add(jobj, "type", j_type);
    json_object_object_add(jobj, "login", j_login);
    json_object_object_add(jobj, "password", j_passwd);
    json_object_object_add(jobj, "nickname", j_nick);
    data = (char *)json_object_to_json_string(jobj);
    write(0, data, strlen(data));                                               /***************/
    write(0, "\n", strlen("\n"));                                               /***************/
    if (write(clnt->sock, data, strlen(data)) == -1) {
        printf("error = %s\n", strerror(errno));
    }

    int res;
    read(clnt->sock, &res, 1024);

    switch (res) {
        case 1: return LG_ERROR_DATA; break;
        case -1: return LG_ERROR_CONECTION; break;
    }
    return NULL;
}

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

void *sing_up_thread(void *param)
{
 // char *res = "incorrect login or password";
    char *res = NULL;
//    char *res = message_connect(((struct reg_info *)param)->ip, ((struct reg_info *)param)->iport);
    //ippppp port
    if(!res)
       res = message_do_sing_up((struct reg_info *)param);
    if(res)
    {
        gtk_label_set_text(GTK_LABEL(statusLabel), res);
        // message_disconnect();
    }
    else
    {
        init_chat_window(((struct reg_info *)param)->login);
        regged_in = 1;
        free(param);
        return param;
    }
    free(param);
    gtk_widget_set_sensitive(regButton, 1);
    return param;
}

void do_reg(GtkWidget *widget, gpointer data)
{
    (void) widget;
    (void) data;
    if(!gtk_widget_get_sensitive(regButton))
        return;
    const gchar *login, *password, *nickname;
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
    ri->login = (char *)login;
    ri->password = (char *)password;
    ri->nickname = (char *)nickname;
    // gtk_widget_hide(signUpWindow);
    // gtk_widget_show_all(StartWindow);
    pthread_create(&reginer, 0, sing_up_thread, (void *)ri);
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
    statusLabel = GTK_WIDGET(gtk_builder_get_object(builder,"StatusLabel"));
    g_signal_connect(G_OBJECT(regButton),"clicked", G_CALLBACK(do_reg),NULL);
    regged_in = 0;
    // g_timeout_add(50, check_login, 0);
}
