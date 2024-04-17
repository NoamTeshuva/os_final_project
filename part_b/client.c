// Simplified client.c for demonstration
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>



/*
 * Client program that generates and sends random numbers to a server to check for primality.
 * Usage: ./client <server_ip> <seed>
 * The client uses the seed to generate a series of random numbers, sends them to the server, and prints the server's response.
 * Compile with: gcc client.c -o client
 */



int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <server_ip> <seed>\n", argv[0]);
        return 1;
    }

    // Initialize random seed
    int seed = atoi(argv[2]);
    srand(seed);

    // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Define server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9001);
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        close(sock);
        return 1;
    }

    // Send and receive data
    for (int i = 0; i < 10; i++) {
        long long num = rand() % 100; // Using smaller numbers for demonstration
        char buffer[1024];
        snprintf(buffer, sizeof(buffer), "%lld", num);
        send(sock, buffer, strlen(buffer), 0);

        // Receive response
        int len = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (len > 0) {
            buffer[len] = '\0'; // Ensure null-terminated string
            printf("Server: %s\n", buffer);
        } else {
            printf("Failed to receive data\n");
        }
    }

    close(sock);
    return 0;
}
