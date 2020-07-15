#include "server.h"

static char *mx_proc_log_in(json_object *jobj);
static char *mx_proc_reg(json_object *jobj);
char *mx_pars_client_mess(json_object *jobj);

void mx_socket_handler(int client_sock) {
    char buffer[1024];
    json_object *jobj = NULL;
    char *ret = 0;
    ssize_t res = read(client_sock, buffer, 1024);

    if (res <= 0) {
        return;
    }
    jobj = json_tokener_parse(buffer);
    ret = mx_pars_client_mess(jobj);
    write(0, ret, strlen(ret));
    if (ret && res > 0)
        res = write(client_sock, ret, strlen(ret));



//        printf("Send %s\n", buffer);
//    json_object_put(jobj);
}

char *mx_pars_client_mess(json_object *jobj) {
    const char *type = json_object_get_string(json_object_object_get(jobj, "type"));
    
    if (strcmp(type, "log_in") == 0)
        return mx_proc_log_in(jobj);
    else if (strcmp(type, "sing_up") == 0)
        return mx_proc_reg(jobj);
    // else if (strcmp(type, "do_message") == 0)
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

// ============= Test User ============

char *l = "admin";
char *n = "ogur4ik";
char *p = "pass";



static char *mx_proc_reg(json_object *jobj) {
    const char *log = json_object_get_string(json_object_object_get(jobj, "login"));
    const char *nick = json_object_get_string(json_object_object_get(jobj, "nickname"));
    json_object *jback = json_object_new_object();
    json_object *j_type = json_object_new_string("sign_up_back");
    json_object *j_error = NULL;

    if (strcmp(log, l) != 0 && strcmp(nick, n) != 0) {
        j_error = json_object_new_int(0);
    }
    else {
        j_error = json_object_new_int(1);
    }
    json_object_object_add(jback, "type", j_type);
    json_object_object_add(jback, "error", j_error);
    return (char *)json_object_to_json_string(jback);
}



static char *mx_proc_log_in(json_object *jobj) {
	const char *log = json_object_get_string(json_object_object_get(jobj, "login"));
	const char *pass = json_object_get_string(json_object_object_get(jobj, "password"));
    json_object *jback = json_object_new_object();
    json_object *j_type = json_object_new_string("log_in_back");
    json_object *j_error = NULL;
    json_object *j_nickname = NULL;

	if (strcmp(log, l) == 0 && strcmp(pass, p) == 0) {
        j_error = json_object_new_int(0);
        j_nickname = json_object_new_string(n);
    }
	else {
        write(0, "1!!!!!!!!\n", 10);
		j_error = json_object_new_int(1);
        write(0, "2!!!!!!!!\n", 10);
        j_nickname = json_object_new_string("");
        write(0, "3!!!!!!!!\n", 10);
    }
    json_object_object_add(jback, "type", j_type);
    json_object_object_add(jback, "error", j_error);
    json_object_object_add(jback, "nickname", j_nickname);
    write(0, "4!!!!!!!!\n", 10);
    return (char *)json_object_to_json_string(jback);
}


// static int *mx_proc_masseg(json_object *jobj) {
// 	char buf;

// 	return 0;
// }
