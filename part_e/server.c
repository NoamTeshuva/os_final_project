#include "../part_d/proactor.h"
#include "../Miller-Rabin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 8090
#define PRIME_TEST_ITERATIONS 5  // Adjust based on desired accuracy vs performance trade-off

// Shared variables for prime number tracking
long long int largest_prime = -1;
int largest_prime_index = -1;
int current_index = 0;

pthread_mutex_t lock;

// Event handler function
void prime_handler(int fd) {
    char buffer[1024];
    int n = read(fd, buffer, 1024);
    if (n > 0) {
        buffer[n] = '\0';
        long long int number = atoll(buffer);
        pthread_mutex_lock(&lock);
        printf("I receives %lld, checking if it is prime.\n",number);
        if (is_prime(number, PRIME_TEST_ITERATIONS)) {
            if (number > largest_prime) {
                largest_prime = number;
                largest_prime_index = current_index; // Logic for tracking the index needs refinement
                sprintf(buffer, "%lld Is prime. \nThe largest prime so far is: %lld, In index: %d", number ,largest_prime, largest_prime_index);
            }
        }
        else{
            sprintf(buffer,"%lld Isnt prime",number);
        }
        send(fd, buffer, strlen(buffer), 0);
        current_index++;
        pthread_mutex_unlock(&lock);
    }
    close(fd);
}

int main() {
    pthread_mutex_init(&lock, NULL);

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n",PORT);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            continue;
        }

        // Create a ProactorTask for each new connection
        ProactorTask *task = malloc(sizeof(ProactorTask));
        task->fd = new_socket;
        task->handler = prime_handler;

        // Execute the task using the proactor system
        execute_proactor(task);  // Assuming execute_proactor properly schedules and manages these tasks
    }

    pthread_mutex_destroy(&lock);
    close(server_fd);
    return 0;
}