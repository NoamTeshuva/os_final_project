

#include "../part_d/proactor.h"
#include "../Miller-Rabin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 1234
#define PRIME_TEST_ITERATIONS 5  // Adjust based on desired accuracy vs performance trade-off
#define DIVIDE_BY 100

// Shared variables for prime number tracking
int largest_prime = -1;
int largest_prime_index = -1;
int counter = 0; // Count the amount of numbers that the client sends until now
int condition_met = 0; // Flag to indicate whether the condition is met

pthread_mutex_t lock;
pthread_cond_t cond_divide_100;

// Event handler function
void prime_handler(int fd) {
    char buffer[1024];
    int n = read(fd, buffer, 1024);
    if (n > 0) {
        buffer[n] = '\0';
        pthread_mutex_lock(&lock);
        long long number = atoll(buffer);
        if (is_prime(number, PRIME_TEST_ITERATIONS)) {
            printf("%lld is prime\n", number);
            
            if (number > largest_prime) {
                largest_prime = number;
                largest_prime_index = counter; // Logic for tracking the index needs refinement
            }
        }
        counter++;
            if (counter % DIVIDE_BY == 0) {
                condition_met = 1; // Set the flag to indicate the condition is met
                pthread_cond_signal(&cond_divide_100); // Signal the condition variable
            }
        sprintf(buffer, "__Largest Prime: %d,__\n __Index: %d__\n ", largest_prime, largest_prime_index);
        send(fd, buffer, strlen(buffer), 0); // Send response to the client
        pthread_mutex_unlock(&lock);

    }
    close(fd);
}

void *print_largest_prime() {
    pthread_mutex_lock(&lock);
    while (1) {
        while (!condition_met) {
            pthread_cond_wait(&cond_divide_100, &lock); // Wait for the condition to be signaled
        }
        printf("Largest Prime Until Now Is: %d\n", largest_prime);
        condition_met = 0; // Reset the flag after handling the condition
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond_divide_100, NULL);

    // Create a thread for printing the largest prime number
    pthread_t print_thread;
    if (pthread_create(&print_thread, NULL, &print_largest_prime, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket option to allow address reuse
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
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

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
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
    pthread_cond_destroy(&cond_divide_100);
    close(server_fd);
    return 0;
}
