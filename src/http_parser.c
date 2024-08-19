#include "http_parser.h"

/**
* Description:
*   Parse the request line, headers and body from the given raw string request
*   and store them in the request_t struct
*
* Returns:
*   Number of headers
*
* Note: 
*   Request will be modified
*/
int http_req_parser(request_t* request, char* comrequest) {

    char* body = strstr(comrequest, "\r\n\r\n");
    *(body) = '\0';
    body += strlen("\r\n\r\n");

    if (*body != '\0') {
        printf("Body: %s\n", body);
        request->reqbody = (char*)malloc(sizeof(char) * (strlen(body) + 1));
        strcpy(request->reqbody, body);
    }
    else {
        request->reqbody = NULL;
    }

    char target[512];
    char* token = strtok(comrequest, "\r\n"); // request line
    sscanf(token, "%s %s %s", request->reqline.method, target, request->reqline.version);
    
    char* tar_token = strtok(target, "/");
    int i = 0;
    request->reqline.target.target_size = 0;
    while(tar_token != NULL) {
        request->reqline.target.target[i] = (char*)malloc(sizeof(char) * (strlen(tar_token) + 1));
        strcpy(request->reqline.target.target[i], tar_token);

        tar_token = strtok(NULL, "/");
        request->reqline.target.target_size++;
        i++;
    }
    

    i = 0;
    request->headers.num_headers = 0;
    while(1) {

        if(i > MAX_HEADERS - 1) {
            return -1;
        }

        token = strtok(NULL, "\r\n");

        if(token == NULL) {
            request->headers.header[i] = NULL;
            request->headers.value[i] = NULL;
            return 0;
        }

        char header[256];
        char value[256];

        sscanf(token, "%[^:]: %s", header, value);

        request->headers.header[i] = (char*)malloc(sizeof(char) * (strlen(header) + 1));
        request->headers.value[i] = (char*)malloc(sizeof(char) * (strlen(value) + 1));

        strcpy(request->headers.header[i], header);
        strcpy(request->headers.value[i], value);

        i++;
        request->headers.num_headers++;
    }
}

void print_request(request_t* request) {
    printf("\nMethod: %s\n", request->reqline.method);

    char target[512];
    for(int i = 0; i < request->reqline.target.target_size; i++) {
        strcat(target, request->reqline.target.target[i]);
    }
    printf("Target: %s\n", target);

    printf("Version: %s\n", request->reqline.version);
    for(int i = 0; i < request->headers.num_headers; i++) {
        printf("Header: %s\n", request->headers.header[i]);
        printf("Value: %s\n", request->headers.value[i]);
    }
    printf("Body: %s\n\n", request->reqbody);
}

void free_request(request_t* request) {
    for (int i = 0; i < request->headers.num_headers; i++) {
        free(request->headers.header[i]);
        free(request->headers.value[i]);
    }
    for(int i = 0; i < request->reqline.target.target_size; i++) {
        free(request->reqline.target.target[i]);
    }
    free(request->reqbody);
}
