#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For file descriptors and sleep
#include <pthread.h>
#include <poll.h> // For pollfd struct and poll function
#include <string.h> // To use strlen
#include "proactor.h" // Include the header for the proactor pattern

// Handler function for handling multiplication of two integers from file descriptor
void multiply_handler(int fd) {
    char buffer[256];
    int num1, num2, product;
    ssize_t nbytes;

    // Prompt for two integers
    printf("Enter two integers separated by space: ");
    fflush(stdout);  // Ensure the prompt is displayed immediately

    nbytes = read(fd, buffer, sizeof(buffer) - 1); // Try to read from stdin
    if (nbytes > 0) {
        buffer[nbytes] = '\0'; // Null-terminate the string
        if (sscanf(buffer, "%d %d", &num1, &num2) == 2) {
            product = num1 * num2;
            printf("Product of %d and %d is %d\n", num1, num2, product);
        } else {
            printf("Error reading integers.\n");
        }
    } else if (nbytes == 0) {
        printf("No more data (EOF).\n");
    } else {
        perror("Read error"); // If read returns -1, an error occurred
    }
}

// Main function sets up a proactor task and runs it
int main() {
    int fd = 0; // Standard input as file descriptor
    ProactorTask task = {fd, multiply_handler}; // Creating a ProactorTask instance with the new handler

    execute_proactor(&task); // Execute the proactor to handle asynchronous I/O

    // Keep the main thread running
    while (1) {
        sleep(1); // Sleep to prevent the program from exiting immediately
    }
    return 0;
}
