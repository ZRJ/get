#include "url.h"
#include "logger.h"

#include <string.h>
#include <memory.h>

int parse_url(const char *url, struct url *parse_result) {
    if (url == NULL) {
        logger("url is null pointer");
        return -1;
    }
    char url_cpy[1024];
    strcpy(url_cpy, url);

    parse_result->url = (char *)malloc(sizeof(char)*strlen(url_cpy));
    if (parse_result->url == NULL) {
        logger("malloc url failed");
        return -1;
    }
    strcpy(parse_result->url, url_cpy);
    logger("the original url is");
    logger(parse_result->url);

    char *protocal_begin_pos = strstr(url_cpy, "://");;
    char *protocal_end_pos;
    if (protocal_begin_pos == NULL) {
        protocal_end_pos = url_cpy;
        logger("use default url scheme");
        parse_result->scheme = URL_SCHEME_HTTP;
    } else {
        protocal_end_pos = protocal_begin_pos + 3;

        char url_protocal[10] = {0};
        strncpy(url_protocal, url_cpy, protocal_begin_pos-url_cpy);
        logger("the protocal is");
        logger(url_protocal);

        if (strcmp(url_protocal, "http") == 0) {
            parse_result->scheme = URL_SCHEME_HTTP;
        } else if (strcmp(url_protocal, "https") == 0) {
            parse_result->scheme = URL_SCHEME_HTTPS;
        } else if (strcmp(url_protocal, "ftp") == 0) {
            parse_result->scheme = URL_SCHEME_FTP;
        } else {
            logger("unknow url scheme");
            return -1;
        }
    }
 
    strcpy(url_cpy, protocal_end_pos);
    logger("after sub string");
    logger(url_cpy);

    char host_and_port[1024] = {0};
    char *host_and_port_end_pos = strstr(url_cpy, "/");
    if (host_and_port_end_pos == NULL) {
        // only main domian, without request path
        strcpy(host_and_port, url_cpy); 
    } else {
        int host_and_port_len = host_and_port_end_pos - url_cpy;
        strncpy(host_and_port, url_cpy, host_and_port_len);
    }
    char *port_pos = strstr(host_and_port, ":");
    if (port_pos == NULL) {
        parse_result->host = (char *)malloc(sizeof(char)*strlen(host_and_port));
        if (parse_result->host == NULL) {
            logger("malloc host failed");
            return -1;
        }
        strcpy(parse_result->host, host_and_port);
        logger("the host is");
        logger(parse_result->host);

        logger("use default port");
        parse_result->port = 80;

        return 0;
    } else {
        int host_len = port_pos - host_and_port;
        parse_result->host = (char *)malloc(sizeof(char)*host_len);
        if (parse_result->host == NULL) {
            logger("malloc host failed");
            return -1;
        }
        strncpy(parse_result->host, url_cpy, host_len);
        *(parse_result->host + host_len) = '\0';
        logger("the host is");
        logger(parse_result->host);

        strcpy(url_cpy, port_pos+1);
        logger("after sub string, the port in str is");
        logger(url_cpy);
        parse_result->port = atoi(url_cpy);
    }
    return 0;
}
