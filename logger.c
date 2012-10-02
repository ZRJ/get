#include <stdio.h>
#include <time.h>
#include "logger.h"

int logger(char *msg) {
    time_t now = time(NULL);
    char buffer[80];
    strftime(buffer, 24, "%Y-%m-%d %H:%M:%S", localtime(&now));
    return printf("[%s]%s\n", buffer, msg);
}
