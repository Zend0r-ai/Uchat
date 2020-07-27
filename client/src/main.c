#include "../inc/client.h"

t_client_info *get_client_info(void) {
    static t_client_info argv;

    return &argv;
}



void mx_print_usage(int num) {

    switch (num) {
        case 0:
            printf("usage: ./uchat [ip] [port (> 1024)]\n");
            break;
        case 1: {
            printf("usage: ./uchat");
            printf("\033[1;91m [ip]\e[0m");
            printf(" [port (> 1024)]\n");
            break;
        }
        case 2: {
            printf("usage: ./uchat [ip]");
            printf("\033[1;91m [port (> 1024)]\n\e[0m");
            break;
        }
    }
    exit(1);
}


bool mx_check_ip(char *argv[], char **d_ip) {
    char *ip = strdup(argv[1]);

    if (mx_count_words(ip, '.') != 4) {
    	*d_ip = NULL;
    	return false;
    }
    *d_ip = ip;
    return true;
}

bool mx_check_port(char *argv[], short *port) {
	*port = atoi(argv[2]);

    if (*port <= 1024)
    	return false;
    return true;
}

t_start *mx_check_input(int argc, char *argv[]){
    short port = 0;
    char *ip = NULL;
    t_start *start = NULL;

    if (argc < 3) {
        mx_print_usage(0);
    }
    if (!mx_check_ip(argv, &ip)) {
    	mx_print_usage(1);
    }
    if (!mx_check_port(argv, &port)) {
    	mx_print_usage(2);
    }
    start = (t_start *)malloc(sizeof(t_start));
    start->ip = ip;
    start->port = port;
    return start;
}

int init_connection(int argc,char *argv[], int sock, t_start *start_data) {
    struct sockaddr_in addr;
    tls_cfg = NULL;
    tls_ctx = NULL;

    if (sock == -1) {
        printf("ERROR: %s\n", strerror(errno));
        return -1;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(start_data->port);
    addr.sin_addr.s_addr = inet_addr(start_data->ip);
    inet_aton(start_data->ip, &addr.sin_addr);
    if (tls_init() == -1)
        errx(1, "unable to initialize TLS");
    if ((tls_cfg = tls_config_new()) == NULL)
        errx(1, "unable to allocate TLS config");
    tls_config_insecure_noverifyname(tls_cfg);
    if (tls_config_set_dheparams(tls_cfg, "legacy") == -1)
        errx(1,"unable to set dheparams");
    if (tls_config_set_ca_file(tls_cfg, "./rcirtificate/root.pem") == -1)
        errx(1, "unable to set root CA file root.pem");
    if (tls_config_set_cert_file(tls_cfg, "client.pem") == -1)
        errx(1, "unable to set TLS certificate file client.pem");
    if (tls_config_set_key_file(tls_cfg, "client.key") == -1)
        errx(1, "unable to set TLS key file client.key");

    if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) != 0) {
        printf("CONNECT ERROR: %s\n", strerror(errno));
        close(sock);
        return -1;
    }
    if ((tls_ctx = tls_client()) == NULL)
        errx(1, "tls client creation failed");
    // tls_config_insecure_noverifyname(tls_cfg);
    if (tls_configure(tls_ctx, tls_cfg) == -1)
        errx(1, "tls configuration failed (%s)",
            tls_error(tls_ctx));
    if (tls_connect_socket(tls_ctx, sock, "name") == -1) {
        errx(1, "tls connection failed (%s)",
            tls_error(tls_ctx));
    }
    int i = 0;
    if ((i = tls_handshake(tls_ctx)) == -1)
            errx(1, "tls handshake failed (%s)", tls_error(tls_ctx));
    // mx_report_tls(tls_ctx, "uchat");
    return 0;
}

int main(int argc,char *argv[]){
    t_client_info *info = get_client_info();
    info->argv = argv;
    owner.last_server_back = NULL;
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    info->socket = sock;
    start_data = mx_check_input(argc, argv);
    if (init_connection(argc, argv, sock, start_data) < 0)
        return -1;
    gtk_init(&argc, &argv);
    init_start_window();
    gtk_widget_show_all(StartWindow);
    gtk_main();
    close(sock);
    return 0;
}

