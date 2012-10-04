// some chagne

#include "logger.h"
#include "url.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAX_DATA_SIZE 65535

#define SERVER_PORT 80

int main(int argc, char **argv) {
    if (argc < 2) {
        logger("Usage: get <donmain>, eg. get baidu.com");
        return 1;
    }
    logger("begin");

    struct url parse_result;
    if (parse_url(argv[1], &parse_result) == -1) {
        return 1;
    }

    char buffer[MAX_DATA_SIZE];
    int sockfd, numbytes;
    struct sockaddr_in server_addr;
    /* get host by name */
    struct hostent *host;
    if ((host=gethostbyname(parse_result.host)) == NULL) {
        logger("get host by name failed");
        return 1;
    }
    if ( (sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        logger("socket error");
        return 1;
    }
    memset(&server_addr, 0, sizeof(struct sockaddr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    // server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    bcopy(host->h_addr, &(server_addr.sin_addr.s_addr), host->h_length);
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        logger("connect error");
        return 1;
    }
    char request_header[1024];
    sprintf(request_header, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", 
                parse_result.path, parse_result.host);
    int request_header_len = strlen(request_header);
    if (send(sockfd, request_header, request_header_len, 0) == -1) {
        logger("send error");
        return 1;
    }
    if ( (numbytes=recv(sockfd, buffer, MAX_DATA_SIZE, 0)) == -1) {
        logger("recv error");
        return 1;
    }
    if (numbytes) {
        buffer[numbytes] = '\0';
        logger(buffer);
    }
    close(sockfd);
    logger("done");
    return 0;
}
