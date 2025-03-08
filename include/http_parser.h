#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HEADERS 50
#define MAX_PATH_SIZE 32

struct reqline {
    char method[8];
    char target[512];
    char version[16];
};

// The maximum number of headers is 50
struct headers {
    int num_headers;
    char* header[MAX_HEADERS + 1];
    char* value[MAX_HEADERS + 1];
};

typedef struct {
    struct reqline reqline;
    struct headers headers;
    char* reqbody;
} request_t;


int http_req_parser(request_t* request, char* comrequest);
void print_request(request_t* request);
void free_request(request_t* request);

#endif