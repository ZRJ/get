#include "logger.h"
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_DATA_SIZE = 65535

#define SERVER_IP "220.181.111.86"
#define SERVER_PORT 80

int main(int argc, char **argv) {
    logger("begin");
    return 0;
}
