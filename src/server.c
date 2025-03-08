#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "http_parser.h"

#define PORT 4221

int main() {
	// Disable output buffering
	setbuf(stdout, NULL);
 	setbuf(stderr, NULL);
	
	int server_fd, client_addr_len;
	struct sockaddr_in client_addr;

	char reqbuf[1024] = {0};

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		printf("Socket creation failed: %s...\n", strerror(errno));
		return 1;
	}

	int reuse = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		printf("SO_REUSEADDR failed: %s \n", strerror(errno));
		return 1;
	}

	struct sockaddr_in serv_addr = { .sin_family = AF_INET ,
									 .sin_port = htons(PORT),
									 .sin_addr = { htonl(INADDR_ANY) },
									};

	if (bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0) {
		printf("Bind failed: %s \n", strerror(errno));
		return 1;
	}

	int connection_backlog = 5;
	if (listen(server_fd, connection_backlog) != 0) {
		printf("Listen failed: %s \n", strerror(errno));
		return 1;
	}

	printf("Waiting for a client to connect...\n");
	client_addr_len = sizeof(client_addr);

    request_t req = {0};
	while(1) {
        req = (request_t){0};

		int fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
		printf("Client connected\n");

		char client_ip[INET_ADDRSTRLEN];
    	inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    	int client_port = ntohs(client_addr.sin_port);

		printf("Connection accepted from %s:%d\n", client_ip, client_port);

		read(fd, reqbuf, 1024 - 1);
        http_req_parser(&req, reqbuf);
        print_request(&req);

		close(fd);
	}
	
	close(server_fd);

	return 0;
}
