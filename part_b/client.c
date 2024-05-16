//Compile: gcc -o client client.c
//Run: ./client
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h> // Include for time function

#define PORT 1834
#define IP "127.0.28.1"


int main() {
    // Use current time as seed for random generator
    srand(time(NULL));
    
    // Default server IP address
    char *server_ip = IP;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Define server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(server_ip);

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        close(sock);
        return 1;
    }

    // Send and receive data
    for (int i = 0; i < 10; i++) {
        long long num = rand() % 100; // Using smaller numbers for demonstration
        long long seed = time(NULL); // Generate a seed
        char buffer[1024];
        snprintf(buffer, sizeof(buffer), "%lld %lld", num, seed); // Send both num and seed
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