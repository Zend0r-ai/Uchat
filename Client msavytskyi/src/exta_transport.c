#include "client.h"

int mx_socket(int domain, int type, int protocol) {
	int res = socket(domain, type, protocol);
	if (res == -1) {
		perror("Socket faild");
		exit(1);
	}
	return res;
}

int mx_bind(int socket, const struct sockaddr *address, socklen_t address_len){
	int res = bind(socket, address, address_len);
	if (res == -1) {
		perror("Bind faild");
		exit(1);
	}
	return res;
}

int mx_listen(int socket, int backlog) {
	int res = listen(socket, backlog);
	if (res == -1) {
		perror("Listen faild");
		exit(1);
	}
	return res;
}

int mx_accept(int socket, struct sockaddr *restrict address,
				socklen_t *restrict address_len) {
	int res = accept(socket, address, address_len);
	if (res == -1) {
		perror("Accept faild");
		exit(1);
	}
	return res;
}
