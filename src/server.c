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


int main() {
	// Disable output buffering
	setbuf(stdout, NULL);
 	setbuf(stderr, NULL);

	// You can use print statements as follows for debugging, they'll be visible when running tests.
	printf("Logs from your program will appear here!\n");

	// Uncomment this block to pass the first stage
	//
	int server_fd, client_addr_len;
	struct sockaddr_in client_addr;
	
	//char* response200 = "HTTP/1.1 200 OK\r\n\r\n";
	//char* response404 = "HTTP/1.1 404 Not Found\r\n\r\n";

	char reqbuf[1024] = {0};

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		printf("Socket creation failed: %s...\n", strerror(errno));
		return 1;
	}

	// Since the tester restarts your program quite often, setting SO_REUSEADDR
	// ensures that we don't run into 'Address already in use' errors
	int reuse = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		printf("SO_REUSEADDR failed: %s \n", strerror(errno));
		return 1;
	}

	struct sockaddr_in serv_addr = { .sin_family = AF_INET ,
									 .sin_port = htons(4221),
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

	while(1) {
		int fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
		printf("Client connected\n");

		char client_ip[INET_ADDRSTRLEN];
    	inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    	int client_port = ntohs(client_addr.sin_port);

		printf("Connection accepted from %s:%d\n", client_ip, client_port);
		read(fd, reqbuf, 1024 - 1);

		request_t comrequest = {0};
		if(http_req_parser(&comrequest, reqbuf)) {
			printf("Number of headers exceeds 50!\n");
			return -1;
		}

		print_request(&comrequest);

		//char* reqline = strtok(reqbuf, "\r\n");
		//strtok(reqline, " ");
		//char* reqtarget = strtok(NULL, " ");
//
		//char* token = strtok(reqtarget, "/");
		//
		//if(token == NULL || strcmp(token, "echo") != 0) {
		//	send(fd, response404, strlen(response404), 0);
		//}
		//else {
		//	char* message = strtok(NULL, "/");
//
		//	if(message == NULL) {
		//		send(fd, response404, strlen(response404), 0);
		//	}
		//	else if(strtok(NULL, "/") == NULL) {
		//		char responsemsg[1024];
		//		snprintf(responsemsg, 1024, 
		//		"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %d\r\n\r\n%s", strlen(message), message);
		//		//printf("%s\n", responsemsg);
		//		send(fd, responsemsg, strlen(responsemsg), 0);
		//	}
		//	else {
		//		send(fd, response404, strlen(response404), 0);
		//	}
		//}
		clean_request(&comrequest);
		close(fd);
	}
	
	close(server_fd);

	return 0;
}
