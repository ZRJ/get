#include "logger.h"
#include "url.h"
#include "header.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define BUFFER_DATA_SIZE 1024
#define SERVER_PORT 80

pthread_t thread[2];
pthread_mutex_t mut;

struct thread_param {
    char url[1024];
    char range[2][10];
};

void* download_thread(void *param) {
    logger("in download thread");

    logger("locking");
    pthread_mutex_lock(&mut);

    struct thread_param *request_param = (struct thread_param *)param;
    
    // parse url and get ip address
    struct url parse_result;
    if (parse_url(request_param->url, &parse_result) == -1) {
        pthread_exit(NULL);
    }
    struct hostent *host;
    if ((host=gethostbyname(parse_result.host)) == NULL) {
        logger("get host by name failed");
        pthread_exit(NULL);
    }

    // init socket
    int sockfd;
    if ( (sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        logger("socket error");
        pthread_exit(NULL);
    }
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(struct sockaddr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    // server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    bcopy(host->h_addr, &(server_addr.sin_addr.s_addr), host->h_length);

    // connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        logger("connect error");
        pthread_exit(NULL);
    }

    // bulid request header
    char request_header[1024];
    sprintf(request_header, "GET %s HTTP/1.1\r\nHost: %s\r\nRange: bytes=%s-%s\r\nConnection: close\r\n\r\n", 
                parse_result.path, parse_result.host, request_param->range[0], request_param->range[1]);
    logger("request header is");
    logger(request_header);
    int request_header_len = strlen(request_header);

    // send request header
    if (send(sockfd, request_header, request_header_len, 0) == -1) {
        logger("send error");
        pthread_exit(NULL);
    }
    
    // open a file to storage data
    FILE *fp = fopen("download", "a+");
    if (fp == NULL) {
        logger("open file failed");
        pthread_exit(NULL);
    }

    // seek to the begin position
    fseek(fp, atoi(request_param->range[0]), SEEK_SET);

    // get response header
    char buffer[BUFFER_DATA_SIZE] = {0};
    int numbytes = recv(sockfd, buffer, BUFFER_DATA_SIZE, 0);
    if (numbytes == -1) {
        logger("recv error");
        pthread_exit(NULL);
    }
    char *header_end_pos = strstr(buffer, "\r\n\r\n");
    if (header_end_pos == NULL) {
        logger("can not find header");
        pthread_exit(NULL);
    }
    char response_header[1024] = {0};
    int header_len = header_end_pos - buffer;
    memcpy(response_header, buffer, header_len);
    logger("response header is");
    logger(response_header);

    // get response content length
    int response_content_length = get_content_length(response_header);
    int writen_count = 0;

    // storage the remian
    fwrite(header_end_pos+4, numbytes-header_len-4, 1, fp);
    fflush(fp);
    writen_count += numbytes - header_len - 4;

    // storage response body
    while((numbytes=recv(sockfd, buffer, BUFFER_DATA_SIZE, 0))) {
        if (numbytes == -1) {
            logger("recv error");
            pthread_exit(NULL);
        }
        fwrite(buffer, numbytes, 1, fp);
        fflush(fp);
        writen_count += numbytes;
        char write_process[100] = {0};
        sprintf(write_process, "looping write %d/%d, download range is %s - %s", 
                writen_count, response_content_length, request_param->range[0], request_param->range[1]);
        logger(write_process);
    }
    logger("write done");
    fclose(fp);
    
    logger("freeing lock");
    pthread_mutex_unlock(&mut);

    close(sockfd);

    logger("finish download thread");
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        logger("Usage: get <donmain>, eg. get baidu.com");
        return 1;
    }

    logger("begin");

    pthread_mutex_init(&mut, NULL);
    memset(&thread, 0, sizeof(thread));

    // begin thread 0
    struct thread_param param0;
    strcpy(param0.url, argv[1]);
    strcpy(param0.range[0], "0");
    strcpy(param0.range[1], "29999");
    if (pthread_create(&thread[0], NULL, download_thread, (void *)(&param0)) != 0) {
        logger("create thread failed");
        return 1;
    }
    sleep(2);
    
    // begin thread 1
    struct thread_param param1;
    strcpy(param1.url, argv[1]);
    strcpy(param1.range[0], "30000");
    strcpy(param1.range[1], "");
    if (pthread_create(&thread[1], NULL, download_thread, (void *)(&param1)) != 0) {
        logger("create thread failed");
        return 1;
    }

    // waiting for threads end
    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);

    logger("done");

    return 0;
}
