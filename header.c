#include "logger.h"

#include <string.h>
#include <stdlib.h>

int get_content_length(const char *header) {
    if (header == NULL || strlen(header) > 1024) {
        return -1;
    }
    char h[1024] = {0};
    strcpy(h, header);

    char *content_length_begin_pos = strstr(h, "Content-Length");
    if (content_length_begin_pos == NULL) {
        content_length_begin_pos = strstr(h, "content-length");
    }
    if (content_length_begin_pos == NULL) {
        logger("can not find content length");
        return -1;
    }
    char tmp[1024] = {0};
    strcpy(tmp, content_length_begin_pos+strlen("Content-Length: "));
    strcpy(h, tmp);
    
    char *content_length_end_pos = strstr(h, "\r\n");
    *content_length_end_pos = '\0';
    logger("the content length is");
    logger(h);

    return atoi(h);
}
