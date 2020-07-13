#include "../inc/client.h"

t_client_info *get_client_info(void) {
    static t_client_info socket;

    return &socket;
}

int main(int argc,char *argv[]){
	argc = 0;
    argv = NULL;
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1) {
        printf("error = %s\n", strerror(errno));
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5001);
    inet_aton("127.0.0.1", &addr.sin_addr);
    if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) != 0) {
        printf("connect error = %s\n", strerror(errno));
        close(sock);
        return -1;
    }
    gtk_init(&argc, &argv);
    init_start_window();
    gtk_widget_show_all(StartWindow);
    gtk_main();
    close(sock);
    return 0;
}

