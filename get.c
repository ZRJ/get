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
    logger("done");
    return 0;
}
