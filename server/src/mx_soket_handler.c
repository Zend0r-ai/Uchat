#include "server.h"

static int mx_count_array(void *arr, int data);
static char *mx_sign_in(json_object *jobj, int *connected_users,int fd);
static char *mx_sign_up(json_object *jobj);
char *mx_parse_client_message(json_object *jobj, int *connected_users, int fd);

void mx_socket_handler(int client_sock, int *connected_users) {
    char buffer[1024];
    json_object *jobj = NULL;
    char *ret = 0;
    
    ssize_t res = read(client_sock, buffer, 1024);

    if (res <= 0)
        return;
    
    jobj = json_tokener_parse(buffer);
    ret = mx_parse_client_message(jobj, connected_users, client_sock);
    write(0, ret, strlen(ret)); // DEBUG string
    
    if (ret)
        res = write(client_sock, ret, strlen(ret));
}

char *mx_parse_client_message(json_object *jobj, int *connected_users, int fd) {
    const char *type = json_object_get_string(json_object_object_get(jobj, "type"));
    
    if (strcmp(type, "log_in") == 0)
        return mx_sign_in(jobj, connected_users, fd);
    else if (strcmp(type, "sing_up") == 0)
        return mx_sign_up(jobj);
    // else if (strcmp(type, "new_message") == 0)
    //     return mx_proc_do_message(jobj);
    return NULL;
}


//int interact(int client_sock) {
//    int buffer = 0;
//
//    ssize_t size = read(client_sock, &buffer, sizeof(buffer));
//    if (size <= 0)
//        return (int) size;
//    printf("Received %d\n", buffer);
//    buffer++;
//    size = write(client_sock, &buffer, sizeof(buffer));
//    if (size > 0)
//        printf("Sent %d\n", buffer);
//    return (int) size;
//}

static char *mx_sign_up(json_object *jobj) {
    const char *log = json_object_get_string(json_object_object_get(jobj, "login"));
    const char *pass = json_object_get_string(json_object_object_get(jobj, "password"));
    const char *nick = json_object_get_string(json_object_object_get(jobj, "nickname"));
    json_object *jback = json_object_new_object();
    json_object *j_type = json_object_new_string("sign_up_back");
    json_object *j_error = NULL;

    t_user user = {log, pass, nick};
    int check = db_check_login_nickname(&db, user);

    if (check == -1) {
        j_error = json_object_new_int(1); // incorrect login
    } else if (check == -2) {
        j_error = json_object_new_int(2); // incorrect nickname
    } else if (check == 0) {
        db_add_user(&db, user);
        j_error = json_object_new_int(0);
    }

    json_object_object_add(jback, "type", j_type);
    json_object_object_add(jback, "error", j_error);
    
    return (char *)json_object_to_json_string(jback);
}

static char *mx_sign_in(json_object *jobj, int *connected_users, int fd) {
	const char *log = json_object_get_string(json_object_object_get(jobj, "login"));
	const char *pass = json_object_get_string(json_object_object_get(jobj, "password"));
    json_object *jback = json_object_new_object();
    json_object *j_type = json_object_new_string("log_in_back");
    json_object *j_error = NULL;
    json_object *j_nickname = NULL;
    json_object *j_id = NULL;
    t_user user = {log, pass, NULL};

    int user_id = db_verify_user(&db, user);
    char *user_nickname = db_get_user_nickname(&db, user_id);
    int is_connected = mx_count_array(connected_users, user_id);
    //printf("user_id = %d, mx_count_array = %d\n", user_id, mx_count_array(connected_users, user_id));
    
	if (user_id > 0 && user_nickname != NULL && is_connected == 0) {
        connected_users[fd] = user_id;
        //printf("connected_users[%d]=%d\n", fd, mx_count_array(connected_users, user_id));
        j_error = json_object_new_int(0);
        j_nickname = json_object_new_string(user_nickname);
        j_id = json_object_new_int(user_id);
    } else if (user_id <= 0) {
        j_error = json_object_new_int(1);
        j_nickname = json_object_new_string("");
    } else if (is_connected != 0) {
		j_error = json_object_new_int(2);
        j_nickname = json_object_new_string("");
    }
    json_object_object_add(jback, "type", j_type);
    json_object_object_add(jback, "error", j_error);
    json_object_object_add(jback, "nickname", j_nickname);
    json_object_object_add(jback, "user_id", j_id);

    return (char *)json_object_to_json_string(jback);
}

static int mx_count_array(void *arr, int data) {
    int counter = 0;

    for (int i = 0; i < USERS_LIMIT; i++) {
        if (((int *)arr)[i] == data)
            counter++;
    }
    return counter;
}
