#include <stdio.h>
#include <time.h>
#include "logger.h"

int logger(char *msg) {
    time_t now = time(NULL);
    char buffer[30];
    strftime(buffer, 30, "%c", localtime(&now));
    return printf("[%s]%s\n", buffer, msg);
}
