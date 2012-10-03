#ifndef _URL_H_INCLUDED_
#define _URL_H_INCLUDED_

struct url {
    char *url; // the origin url
    enum _scheme {
        URL_SCHEME_HTTP,
        URL_SCHEME_HTTPS,
        URL_SCHEME_FTP
    } scheme;
    char *host;
    int port;
    char *path;
    char *query;
    char *anchor;
};

// parse an url, save the result in the url struct
// return 0 on success and -1 on failed
int parse_url(const char *url, struct url *parse_result);

#endif
