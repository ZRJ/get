#include "logger.h"

#include <string.h>
#include <stdlib.h>

int get_content_length(const char *header) {
    logger("in len fun");
    return 0;

    if (header == NULL || strlen(header) > 1024) {
        return -1;
    }
    char h[1024] = {0};
    strcpy(h, header);

    char *content_length_begin_pos = (char *)(strstr(h, "Content-Length")
                                || strstr(h, "content-length"));
    if (content_length_begin_pos == NULL) {
        return -1;
    }
    strcpy(h, content_length_begin_pos+strlen("content_length:"));
    logger("after moving to length");
    logger(h);
    
    char *content_length_end_pos = strstr(h, "\r\n");
    *content_length_end_pos = '\0';
    logger("after deleting others");
    logger(h);

    return atoi(h);
}
