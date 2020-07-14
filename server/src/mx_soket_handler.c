#include "server.h"

static int mx_proc_log_in(json_object *jobj);
static int mx_proc_reg(json_object *jobj);

int mx_socket_handler(int client_sock) {
    char buffer[1024];
    json_object *jobj = NULL;
    const char *type;
    int ret = 0;
    
  	ssize_t res = read(client_sock, buffer, 1024);
    jobj = json_tokener_parse(buffer);
    type = json_object_get_string(json_object_object_get(jobj, "type"));
    if (strcmp(type, "log_in") == 0)
    	ret = mx_proc_log_in(jobj);
    else if (strcmp(type, "sing_up") == 0)
        ret = mx_proc_reg(jobj);
    
    if (res > 0)
        res = write(client_sock, &ret, sizeof(ret));



//        printf("Send %s\n", buffer);
//    json_object_put(jobj);

    return (int) res;
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



static int mx_proc_reg(json_object *jobj) {
    const char *log = json_object_get_string(json_object_object_get(jobj, "login"));
    const char *nick = json_object_get_string(json_object_object_get(jobj, "nickname"));

    if (strcmp(log, l) == 0 || strcmp(nick, n) == 0)
        return 1;
    else
        return 0;
	return 1;
}



static int mx_proc_log_in(json_object *jobj) {
	const char *log = json_object_get_string(json_object_object_get(jobj, "login"));
	const char *pass = json_object_get_string(json_object_object_get(jobj, "password"));

	if (strcmp(log, l) == 0 && strcmp(pass, p) == 0)
		return 0;
	else
		return 1;
}


// static int *mx_proc_masseg(json_object *jobj) {
// 	char buf;

// 	return 0;
// }
