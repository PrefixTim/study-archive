#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    int listenfd = 0, connfd = 0;

    struct sockaddr_in serv_addr;

    char sendBuff[1025];
    char recvBuff[1025];
    int n = 0;
    int numrv;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("socket retrieve success\n");

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));
    memset(recvBuff, '0', sizeof(recvBuff));

    serv_addr.sin_family = AF_INET;
    // serv_addr.sin_addr.s_addr = inet_addr("127.0.0.2");
    // struct hostent *he = gethostbyname("server.timofey.cs164");
    // memcpy(&serv_addr.sin_addr, he->h_addr_list[0], he->h_length);
    // struct hostent *he = gethostbyname("server.timofey.cs164");
    // if (he == NULL) {
        // printf("Failed");
        // return 1;
    // }
    serv_addr.sin_port = htons(5000);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // serv_addr.sin_addr.s_addr = *((unsigned long *)he->h_addr);
    
    bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    if (listen(listenfd, 10) == -1) {
        printf("Failed to listen\n");
        return -1;
    }

    while (1) {
        connfd = accept(listenfd, (struct sockaddr *)NULL, NULL); // accept awaiting request

        if ((n = read(connfd, recvBuff, sizeof(recvBuff) - 1)) > 0) {
            recvBuff[n] = 0;
            if (fputs(recvBuff, stdout) == EOF) {
                printf("\n Error : Fputs error");
            }
            printf("\n");
        }

        memcpy(sendBuff, recvBuff, sizeof(sendBuff));
        ssize_t ignor = write(connfd, sendBuff, strlen(sendBuff));

        close(connfd);
        sleep(1);
    }

    return 0;
}
