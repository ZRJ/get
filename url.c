#include "url.h"
#include "logger.h"

#include <string.h>
#include <memory.h>

int parse_url(const char *url, struct url *parse_result) {
    if (url == NULL) {
        logger("url is null pointer");
        return -1;
    }

    parse_result->url = (char *)malloc(sizeof(char)*strlen(url));
    if (parse_result->url == NULL) {
        logger("malloc url failed");
        return -1;
    }
    strcpy(parse_result->url, url);
    logger("the original url is");
    logger(parse_result->url);

    if (strstr(url, "://") == NULL) {
        logger("use default url scheme");
        parse_result->scheme = URL_SCHEME_HTTP;    
    } else {
        char url_protocal[10];
        strncpy(url_protocal, url, strstr(url, "://")-url);
        logger("the protocal is");
        logger(url_protocal);
    }

    return 0;
}
