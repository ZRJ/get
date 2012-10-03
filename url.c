#include "url.h"
#include "logger.h"

#include <string.h>
#include <memory.h>

int parse_url(const char *url, struct url *rst) {
    if (url == NULL) {
        logger("url is null pointer");
        return -1;
    }
    char url_cpy[1024];
    strcpy(url_cpy, url);
    
    char *url_scheme_begin_pos = strstr(url_cpy, "://");
    if (url_scheme_begin_pos) {
        // cut out the scheme
        strcpy(url_cpy, url_scheme_begin_pos+3);
        logger("after delete scheme");
        logger(url_cpy);
    }

    return 0;
}
