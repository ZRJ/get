#include "logger.h"
#include "url.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFER_DATA_SIZE 1024
#define SERVER_PORT 80

int main(int argc, char **argv) {
    if (argc < 2) {
        logger("Usage: get <donmain>, eg. get baidu.com");
        return 1;
    }

    logger("begin");
    
    // parse url and get ip address
    struct url parse_result;
    if (parse_url(argv[1], &parse_result) == -1) {
        return 1;
    }
    struct hostent *host;
    if ((host=gethostbyname(parse_result.host)) == NULL) {
        logger("get host by name failed");
        return 1;
    }

    // init socket
    int sockfd;
    if ( (sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        logger("socket error");
        return 1;
    }
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(struct sockaddr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    // server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    bcopy(host->h_addr, &(server_addr.sin_addr.s_addr), host->h_length);

    // connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        logger("connect error");
        return 1;
    }

    // bulid request header
    char request_header[1024];
    sprintf(request_header, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", 
                parse_result.path, parse_result.host);
    int request_header_len = strlen(request_header);

    // send request header
    if (send(sockfd, request_header, request_header_len, 0) == -1) {
        logger("send error");
        return 1;
    }
    
    // open a file to storage data
    FILE *fp = fopen("download", "w+");
    if (fp == NULL) {
        logger("open file failed");
        return 1;
    }

    // get response header
    char response_buffer[1024] = {0};
    int numbytes = recv(sockfd, response_buffer, sizeof(response_buffer), 0);
    if (numbytes == -1) {
        logger("recv error");
        return 1;
    }
    char *header_end_pos = strstr(response_buffer, "\r\n\r\n");
    if (header_end_pos == NULL) {
        logger("can not find header");
        return 1;
    }
    char response_header[1024] = {0};
    int header_len = header_end_pos - response_buffer;
    strncpy(response_header, response_buffer, header_len);
    logger("header is");
    logger(response_header);

    // storage the remian
    fwrite(header_end_pos+4, numbytes-header_len-4, 1, fp);
    fflush(fp);

    // storage response body
    char buffer[BUFFER_DATA_SIZE];
    while(numbytes=recv(sockfd, buffer, BUFFER_DATA_SIZE, 0)) {
        if (numbytes == -1) {
            logger("recv error");
            return 1;
        }
        fwrite(buffer, numbytes, 1, fp);
        fflush(fp);
        logger("looping write");
    }

    logger("write done");
    close(sockfd);
    logger("done");
    return 0;
}
