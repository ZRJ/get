#include <stdio.h>
#include <time.h>
#include "logger.h"

int logger(char *msg) {
    time_t tim;
    struct tim *at;
    char now[80];
    time(&tim);
    at = localtime(&tim);
    strftime(now, 79, "%y-%m-%d %H:%M:%S", at);
    return printf("[%s]%s\n", now, msg);
}
