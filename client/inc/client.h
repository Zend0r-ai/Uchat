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
#include <err.h>
#include "../../libmx/inc/libmx.h"
#include "../../libressl-3.2.0/include/tls.h"
#include "../../libressl-3.2.0/include/openssl/evp.h"
#include "../../libressl-3.2.0/include/openssl/aes.h"
#include "../../libressl-3.2.0/include/openssl/sha.h"
#include "../../libressl-3.2.0/include/openssl/conf.h"


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

/* =============== MESSAGE STRUCTURE =============== */

typedef struct s_user_message
{
    unsigned int owner_id;
    char *data;
    time_t tv_id;
    const char *nickname;
} t_user_message;

/* ============= MESSAGE STRUCTURE END ============= */

/* =============== listboxrow STRUCTURE =============== */

typedef struct s_row
{
    t_user_message *message;
    GtkWidget *mess_row;
} t_row;

/* ============= listboxrow STRUCTURE END ============= */

/* =============== USER STRUCTURE =============== */

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
struct tls_config *tls_cfg;
struct tls *tls_ctx;

t_user_info owner;
t_list *history_message_list;
t_list *row_history_list;
// GtkWidget *widget_list;


typedef struct s_client_info {
    int sock;
} t_client_info;

t_client_info *get_client_info(void);

void mx_css_set(GtkCssProvider *cssProvider, GtkWidget *widget);

void *mx_proc_server_back(char *buffer, t_user_info *user);
char *mx_proc_log_in_back(json_object *jobj, t_user_info *user);
char *mx_proc_sign_up_back(json_object *jobj);
t_user_message *mx_proc_message_back(json_object *jobj);
void mx_do_message_request(t_user_message *message, const char *request);

//=======================TLS=====================//
void mx_report_tls(struct tls * tls_ctx, char * host);


#endif
