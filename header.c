#include <string.h>
#include <stdlib.h>

int get_content_length(const char *header) {
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
    
    char *content_length_end_pos = strstr(h, "\r\n");
    *content_length_end_pos = '\0';

    return atoi(h);
}
