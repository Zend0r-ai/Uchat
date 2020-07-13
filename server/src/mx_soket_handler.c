#include "server.h"

int mx_socket_handler(int client_sock) {
    char buffer[1024];

//    json_object *jobj = json_object_new_object();
//    json_object *j_socket = NULL;

    ssize_t res = read(client_sock, &buffer, 1024);
    if (res <= 0)
        printf("%s\n", strerror(errno));
//    j_socket = json_object_new_int(client_sock);

//    jobj = json_tokener_parse(buffer);
//    json_object_object_add(jobj, "Socket", j_socket);



//    res = write(client_sock, &buffer, 1024);
    if (res > 0)
        res = write(client_sock, &buffer, 1024);
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
