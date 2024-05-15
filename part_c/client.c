// 
// compile:gcc -o client client.c 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define MAX_BUFFER 1024

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[MAX_BUFFER];
    int num;

    if(argc != 4) {
        printf("Usage : %s <IP> <port> <client id>\n", argv[0]);
        exit(1);
    }

    // Seed the random number generator
    srand(time(NULL) + atoi(argv[3]));

    // Generate a random number
    num = rand();

    // Create a socket
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(1);
    }

    // Initialize the server address structure
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    if(inet_aton(argv[1], &serv_addr.sin_addr) == 0) {
        perror("inet_aton error");
        exit(1);
    }

    // Connect to the server
    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect error");
        exit(1);
    }

    // Send the random number to the server
    sprintf(buffer, "%d", num);
    if(write(sockfd, buffer, strlen(buffer)) < 0) {
        perror("write error");
        exit(1);
    }

    close(sockfd);
    return 0;
}
