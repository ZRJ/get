#ifndef _URL_H_INCLUDED_
#define _URL_H_INCLUDED_

struct url {
    char *url; // the original url
    enum scheme {
        URL_SCHEME_HTTP,
        URL_SCHEME_HTTPS,
        URL_SCHEME_FTP
    }; // the protocal
    char *host;
    int port;
    char *path; // the path to the file
    char *query; // the request query
    char *anchor;
};

// parse an url, save the result in the url struct
// return 0 on success and -1 on failed
int parse_url(const char *url, struct *url);

#endif
