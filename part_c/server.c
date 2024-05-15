// compile : gcc -o serverC.exe server.c ../Miller-Rabin.c
// run: ./serverC.exe
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <stdbool.h>
#include "../Miller-Rabin.h"
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024
#define PORT 9001

void handle_client(int client_socket, FILE* log_file, long long* largest_prime) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;
    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_received] = '\0'; // Null-terminate received data

        long long number = atoll(buffer);
        bool prime = is_prime(number, 5); // Using Miller-Rabin primality test
        if (prime && number > *largest_prime) {
            *largest_prime = number; // Update largest prime number
        }

fprintf(log_file, "Received number: %lld, Prime: %s, Current highest prime: %lld\n", number, prime ? "Yes" : "No", *largest_prime);
        fflush(log_file); // Flush after each write to ensure it's written to disk

        snprintf(buffer, BUFFER_SIZE, "%lld is %sprime. Largest prime: %lld", number, prime ? "" : "not ", *largest_prime);
        send(client_socket, buffer, strlen(buffer), 0);
    }
    close(client_socket);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    if (server_socket == -1 || bind(server_socket, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1 || listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Server setup error");
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    struct pollfd fds[MAX_CLIENTS];
    fds[0].fd = server_socket;
    fds[0].events = POLLIN;

    FILE* log_file = fopen("server_log.txt", "a");
    if (!log_file) {
        perror("Error opening log file");
        close(server_socket);
        return 1;
    }

    long long largest_prime = 0;

    while (1) {
        if (poll(fds, MAX_CLIENTS, -1) == -1) {
            perror("Poll error");
            break;
        }

        if (fds[0].revents & POLLIN) {
            struct sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof(clientAddr);
            int client_socket = accept(server_socket, (struct sockaddr*)&clientAddr, &clientAddrLen);
            if (client_socket != -1) {
                printf("New connection accepted.\n");
                handle_client(client_socket, log_file, &largest_prime);
            }
        }
    }

    fclose(log_file);
    close(server_socket);
    return 0;
}
