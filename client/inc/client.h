#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/errno.h>
#include <pthread.h>
#include <netdb.h>
#include <unistd.h>
#include <stdbool.h>
#include "../../libjson/json.h"
#include <gtk/gtk.h>
#include <time.h>
#include <gio/gio.h>
#include "../../libmx/inc/libmx.h"


#define BUF_SIZE 2048

/* ======= Sing In defines ======== */
#define LG_ERROR_DATA "Wrong login or password"
#define LG_ERROR_CONECTION "Server conection problem.."
#define LG_ERROR_ONLINE "User already online"

// ======= Sing Up defines ========
#define SU_ERROR_DATA_LOGIN "Login already exist"
#define SU_ERROR_DATA_NICKN "Nickname already exist"
#define SU_ERROR_CONECTION "Server conection problem.."
#define SU_ERROR_CONFIRM_PASS "Wrong password confirm"

void init_chat_window();
extern GtkWidget *chatWindow;

void init_login_window();
extern GtkWidget *loginWindow;

void init_sign_up_window();
extern GtkWidget *signUpWindow;

void init_start_window();
extern GtkWidget *StartWindow;

typedef struct s_row t_row;

/* =============== MESSAGE STRUCTURE =============== */

typedef struct s_user_message
{
    unsigned int owner_id;
    char *data;
    time_t tv_id;
    const char *nickname;
    GtkWidget *row;
} t_user_message;

/* ============= MESSAGE STRUCTURE END ============= */

/* =============== USER STRUCTURE =============== */

typedef struct s_row
{
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
}t_row;

typedef struct s_user_info
{
    char *login;
    bool is_online;
    char *password;
    char *confpass;
    const char *nickname;
    unsigned int id;
    char *last_server_back;
} t_user_info;

/* ============= USER STRUCTURE END ============= */

t_user_info owner;
t_list *history_message_list;
// GtkWidget *widget_list;


typedef struct s_client_info {
    int sock;
} t_client_info;

typedef struct s_edit_data {
    t_user_message *message;
    t_user_message *new_message;
    int index;
} t_edit_data;

t_client_info *get_client_info(void);

void mx_css_set(GtkCssProvider *cssProvider, GtkWidget *widget);

void *mx_proc_server_back(char *buffer, t_user_info *user);
char *mx_proc_log_in_back(json_object *jobj, t_user_info *user);
char *mx_proc_sign_up_back(json_object *jobj);
t_user_message *mx_proc_message_back(json_object *jobj);
void mx_do_message_request(t_user_message *message, const char *request);
void mx_delete_message_row(t_user_message *message, int index);
t_user_message *mx_create_edit_message(t_user_message *message, char *msg_body);
void do_send();
void *read_server_thread(void *par);

#endif
