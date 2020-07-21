#include "server.h"

int main(int argc, const char **argv) {
    //sqlite3 *db;
    argc = 0;
    argv = NULL;
    const char *db_name = "uchat.db";
    int connected_users[USERS_LIMIT];
    struct tls_config *tls_cfg = NULL;
	struct tls *tls_ctx = NULL;
	// struct tls *tls_cctx = NULL;

    //memset(connected_users, 0, USERS_LIMIT);
    for (int i = 0; i < USERS_LIMIT; i++)
        connected_users[i] = 0;
    
    if (tls_init() == -1)
		errx(1, "unable to initialize TLS");
	if ((tls_cfg = tls_config_new()) == NULL)
		errx(1, "unable to allocate TLS config");
    if (tls_config_set_dheparams(tls_cfg, "legacy") == -1)
        errx(1,"unable to set dheparams");
	if (tls_config_set_ca_file(tls_cfg, "server.csr") == -1)
		errx(1, "unable to set root CA file server.csr");
	if (tls_config_set_cert_file(tls_cfg, "server.pem") == -1)
		errx(1, "unable to set TLS certificate file server.csr");
	if (tls_config_set_key_file(tls_cfg, "server.key") == -1)
		errx(1, "unable to set TLS key file server.key");
	if ((tls_ctx = tls_server()) == NULL)
		errx(1, "tls server creation failed");
	if (tls_configure(tls_ctx, tls_cfg) == -1)
		errx(1, "tls configuration failed (%s)", tls_error(tls_ctx));

    // mx_demon();
    int server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server == -1) {
        fprintf(stderr, "error = %s\n", strerror(errno));
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(1111);
    // inet_aton("10.111.3.11", &addr.sin_addr);
    inet_aton("127.0.0.1", &addr.sin_addr);

    int on = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (bind(server, (struct sockaddr *) &addr, sizeof(addr)) != 0) {
        fprintf(stderr, "bind error = %s\n", strerror(errno));
        return -1;
    }
    if (listen(server, 128) == -1) {
        fprintf(stderr, "listen error = %s\n", strerror(errno));
        return -1;
    }
    int kq = kqueue();
    if (kq == -1) {
        fprintf(stderr, "error = %s\n", strerror(errno));
        close(server);
        return -1;
    }
    struct kevent new_event;
    EV_SET(&new_event, server, EVFILT_READ, EV_ADD, 0, 0, 0);
    if (kevent(kq, &new_event, 1, 0, 0, NULL) == -1) {
        fprintf(stderr, "error = %s\n", strerror(errno));
        close(server);
        return -1;
    }

    struct timespec timeout;
    timeout.tv_sec = 1;
    timeout.tv_nsec = 0;
    
    // database init
    db_open(&db, db_name);
    db_init(&db);

    for (;;) {
        int nfds = kevent(kq, NULL, 0, &new_event, 1, NULL);

        if (nfds == -1) {
            fprintf(stderr, "error = %s\n", strerror(errno));
            break;
        }
        if (new_event.ident == (unsigned long)server) {
            int client_sock = accept(server, NULL, NULL);

            if (client_sock == -1) {
                fprintf(stderr, "error = %s\n", strerror(errno));
                break;
            }
            printf("New client, fd=%d\n", client_sock); // DEBUG line
            // printf("users_id connected:"); // DEBUG line
            // for (int i = 3; i < USERS_LIMIT; i++) { // DEBUG line
            //     if (connected_users[i] != 0) { // DEBUG line
            //         printf(" fd[%d]= id[%d]", i, connected_users[i]); // DEBUG line
            //     } // DEBUG line
            // } 
            // printf("\n"); // DEBUG line

            EV_SET(&new_event, client_sock, EVFILT_READ, EV_ADD, 0, 0, 0);
            if (kevent(kq, &new_event, 1, 0, 0, NULL) == -1) {
                fprintf(stderr, "error = %s\n", strerror(errno));
                break;
            }
        } 
        else {
            mx_socket_handler(new_event.ident, connected_users);
            if ((new_event.flags & EV_EOF) != 0) {
                printf("Client disconnected, fd=%lu\n", new_event.ident); // DEBUG line
                connected_users[new_event.ident] = 0;
                //printf("connected_users[%lu], value=%d\n", new_event.ident, connected_users[new_event.ident]);
                close(new_event.ident);
                
            }
        }
    }
    sqlite3_close(db);
    close(kq);
    close(server);

    system("leaks uchat_server");
    return 0;
}
