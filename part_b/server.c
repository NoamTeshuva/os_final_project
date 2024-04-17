#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include "../Miller-Rabin.h" 

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9001);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        close(server_fd);
        return 1;
    }

    printf("Server listening on port 9001...\n");

    while (1) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int client_fd = accept(server_fd, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        char buffer[1024] = {0};
        recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        long long num = atoll(buffer);

        // Using Miller-Rabin primality test
        bool prime = is_prime(num, 5); // Using the Miller-Rabin test with 5 iterations for accuracy
        snprintf(buffer, sizeof(buffer), "%lld is %sprime.", num, prime ? "" : "not ");
        send(client_fd, buffer, strlen(buffer), 0);

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
