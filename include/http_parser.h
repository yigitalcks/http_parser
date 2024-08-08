#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HEADERS 50

struct reqline {
    char method[8];
    char target[256];
    char version[16];
};
// The maximum number of headers is 50
struct headers {
    char* header[MAX_HEADERS + 1];
    char* value[MAX_HEADERS + 1];
    int num_headers;
};

typedef struct {
    struct reqline reqline;
    struct headers headers;
    char* reqbody;
} request_t;

int http_req_parser(request_t* request, char* comrequest);
void print_request(request_t* request);
void clean_request(request_t* request);

#endif