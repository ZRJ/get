#include "url.h"
#include "logger.h"

#include <string.h>
#include <memory.h>

int parse_url(const char *url, struct url *rst) {
    if (url == NULL) {
        logger("url is null pointer");
        return -1;
    }
    if (strlen(url) >= 1024) {
        logger("url too long");
        return -1;
    }
    char url_cpy[1024];
    strcpy(url_cpy, url);
    
    char *scheme_begin_pos = strstr(url_cpy, "://");
    if (scheme_begin_pos) {
        // cut out the scheme
        strcpy(url_cpy, scheme_begin_pos+3);
        logger("after delete scheme");
        logger(url_cpy);
    }

    char *path_begin_pos = strstr(url_cpy, "/");
    if (path_begin_pos == NULL) {
        // only domain
        strcpy(rst->host, url_cpy);
        strcpy(rst->path, "/");
    } else {
        // with path
        int host_len = path_begin_pos - url_cpy;
        strncpy(rst->host, url_cpy, host_len);
        rst->host[host_len] = '\0';
        
        strcpy(rst->path, path_begin_pos);
    }
    logger("parse result");
    logger(rst->host);
    logger(rst->path);
    return 0;
}
