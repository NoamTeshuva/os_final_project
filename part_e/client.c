#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"  // Server IP address
#define SERVER_PORT 8090 // Server port number

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};
    char *message;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    // Prompt user to enter a number
    printf("Enter a number to check if it's prime: ");
    fgets(buffer, 1024, stdin);

    // Send number to the server
    if (send(sock, buffer, strlen(buffer), 0) < 0) {
        perror("Send failed");
        return -1;
    }

    printf("Number sent\n");

    // Receive response from the server
    if (read(sock, buffer, sizeof(buffer)) < 0) {
        perror("Read failed");
        return -1;
    }

    printf("Server response: %s\n", buffer);

    // Close the socket
    close(sock);

    return 0;
}
