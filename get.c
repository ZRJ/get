#include "logger.h"
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_DATA_SIZE 65535

#define SERVER_IP "220.181.111.86"
#define SERVER_PORT 80

int main(int argc, char **argv) {
    logger("begin");
    char buffer[MAX_DATA_SIZE];
    int sockfd, numbytes;
    struct sockaddr_in server_addr;
    /* get host by name */
    struct hsotent *host;
    if ((host=gethostbyname("www.baidu.com")) == NULL) {
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
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        logger("connect error");
        return 1;
    }
    char *request_header = "GET / HTTP/1.1\r\nHost: www.baidu.com\r\n\r\n";
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
