#include "client.h"

t_client_info *get_client_info(void) {
    static t_client_info socket;

    return &socket;
}

static int init_connection(int argc,char *argv[], int sock) {
    t_client_info *inf_sock = get_client_info();
    struct sockaddr_in addr;
    struct tls_config *tls_cfg = NULL;
	struct tls *tls_ctx = NULL;
    u_short port = atoi(argv[2]);

    inf_sock->sock = sock;
    if (sock == -1) {
        printf("ERROR: %s\n", strerror(errno));
        return -1;
    }
    addr.sin_family = AF_INET;
    // addr.sin_port = htons(5003);
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    // inet_aton("10.111.3.11", &addr.sin_addr);
    // inet_aton("127.0.0.1", &addr.sin_addr);
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
    mx_report_tls(tls_ctx, "uchat");
    return 0;
}

int main(int argc,char *argv[]) {
    // argc = 0;
    // argv = NULL;
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (init_connection(argc, argv, sock) < 0)
        return -1;
    gtk_init(&argc, &argv);
    init_start_window();
    gtk_widget_show_all(StartWindow);
    gtk_main();
    close(sock);
    return 0;
}

