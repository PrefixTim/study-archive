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

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("\n Error: no cmd args");
        return 1;
    }

    int sockfd = 0, n = 0;
    char recvBuff[1025];
    char sendBuff[1025];
    struct sockaddr_in serv_addr;

    memset(recvBuff, '0', sizeof(recvBuff));
    memset(sendBuff, '0', sizeof(sendBuff));

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    // struct hostent *he = gethostbyname("server.timofey.cs164");
    // memcpy(&serv_addr.sin_addr, he->h_addr_list[0], he->h_length);
    // serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    struct hostent *he = gethostbyname("server.timofey.cs164");
    if (he == NULL) {
        printf("Failed");
        return 1;
    }
    serv_addr.sin_port = htons(5000);
    serv_addr.sin_addr.s_addr = *((unsigned long *)he->h_addr);
    // printf("Address: %s\n", inet_ntoa(serv_addr.sin_addr));

    // printf("%d", gethostbyname("server.timofey.cs164"));

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\n Error : Connect Failed \n %d", errno);
        return 1;
    }

    memcpy(sendBuff, argv[1], sizeof(argv[1]));
    write(sockfd, sendBuff, strlen(sendBuff));

    while ((n = read(sockfd, recvBuff, sizeof(recvBuff) - 1)) > 0) {
        recvBuff[n] = 0;
        if (fputs(recvBuff, stdout) == EOF) {
            printf("\n Error : Fputs error");
        }
        printf("\n");
    }

    if (n < 0) {
        printf("\n Read Error \n");
    }

    return 0;
}
