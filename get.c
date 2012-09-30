#include "logger.h"
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_DATA_SIZE = 65535

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
    memesett(&server_addr, 0, sizeof(struct sockaddr));

    logger("done");
    return 0;
}
