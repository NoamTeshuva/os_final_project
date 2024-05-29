#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <poll.h>
#include <unistd.h> // For file descriptors
#include "proactor.h"


/*
i know if the fd is hot by using poll()
*/


void *thread_function(void *arg) {
    ProactorTask *task = (ProactorTask *)arg; // cast the arg* whice is
    struct pollfd fds; // is tbuild from two sorts event that determine if it pollin or pollout 
        //POLLOUT is for checking the availabilty
    fds.fd = task->fd;
    fds.events = POLLIN; // Or POLLOUT depending on your needs, Pollin is retruning if data is ready to read

    while (1) {
        int ret = poll(&fds, 1, -1); // Timeout is -1, wait indefinitely
        if (ret > 0) {
            if (fds.revents & POLLIN) { // Check if the fd is  ready for reading
                task->handler(fds.fd);
            }
        } else {
            // Handle error or exit
            perror("Poll error");
            break;
        }
    }
    return NULL;
}

/* 
The Proactor design pattern facilitates asynchronous I/O operations by delegating
the completion of these operations to separate handler objects. It allows an application
to initiate an I/O operation and continue processing without waiting for the operation
to complete. When the I/O operation finishes, the associated handler is invoked to handle
the result. This pattern enhances efficiency and responsiveness 
in systems requiring high concurrency.
 */
void execute_proactor(ProactorTask *task) {
    pthread_t thread;
    pthread_create(&thread, NULL, thread_function, (void *)task);
    pthread_detach(thread); // Optionally detach the thread
}
