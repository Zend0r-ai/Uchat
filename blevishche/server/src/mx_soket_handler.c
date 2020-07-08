#include "server.h"

//int mx_socket_handler(int client_sock) {
//    char *buffer[2048];
//    ssize_t size = read(client_sock, &buffer, 2048);
//    if (size <= 0)
//        return (int) size;
//
//    for (int i = 0; i < ; i++) {
//        if (size > 0)
//            write(client_sock, &buffer, 2048);
//    }
//    return (int) size;
//}
int interact(int client_sock) {
    int buffer = 0;

    ssize_t size = read(client_sock, &buffer, sizeof(buffer));
    if (size <= 0)
        return (int) size;
    printf("Received %d\n", buffer);
    buffer++;
    size = write(client_sock, &buffer, sizeof(buffer));
    if (size > 0)
        printf("Sent %d\n", buffer);
    return (int) size;
}
