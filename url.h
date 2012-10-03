#ifndef _URL_H_INCLUDED_
#define _URL_H_INCLUDED_

struct url {
    char host[512];
    char path[1024];
};

// parse an url, save the result in the url struct
// return 0 on success and -1 on failed
int parse_url(const char *url, struct url *parse_result);

#endif
