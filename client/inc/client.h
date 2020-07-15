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

// ======= Sing In defines ========
#define LG_ERROR_DATA "Wrong login or password"
#define LG_ERROR_CONECTION "Server conection problem.."

// ======= Sing Up defines ========
#define SU_ERROR_DATA "Login or Nickname already exist"
#define SU_ERROR_CONECTION "Server conection problem.."

void init_chat_window();
extern GtkWidget *chatWindow;

void init_login_window();
extern GtkWidget *loginWindow;

void init_sign_up_window();
extern GtkWidget *signUpWindow;

void init_start_window();
extern GtkWidget *StartWindow;

typedef struct s_client_info {
    int sock;
} t_client_info;

t_client_info *get_client_info(void);

void mx_css_set(GtkCssProvider *cssProvider, GtkWidget *widget);

#endif
