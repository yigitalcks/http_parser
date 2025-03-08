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
    
    char* token = strtok(comrequest, "\r\n"); // request line
    sscanf(token, "%s %s %s", request->reqline.method, request->reqline.target, request->reqline.version);

    int i = 0;
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
    printf("Target: %s\n", request->reqline.target);
    printf("Version: %s\n", request->reqline.version);

    printf("Headers: \n");
    for(int i = 0; i < request->headers.num_headers; i++) {
        printf("\t%s: %s\n", request->headers.header[i], request->headers.value[i]);
    }
    printf("Body:\n\t%s\n", request->reqbody);
}

void free_request(request_t* request) {
    for (int i = 0; i < request->headers.num_headers; i++) {
        free(request->headers.header[i]);
        free(request->headers.value[i]);
    }
    free(request->reqbody);
}
