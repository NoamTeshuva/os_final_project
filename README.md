Project Description: Advanced Operating Systems Lab
---------------------------------------------------

### Overview:

This project is designed to explore advanced concepts in operating systems through a series of interconnected tasks involving the Miller-Rabin primality test, client-server architecture, and multi-threading with polling and proactor patterns. The overarching goal is to develop a system capable of identifying prime candidates using the Miller-Rabin test and efficiently handling multiple client requests in parallel.

### Objectives:

*   Implement the Miller-Rabin primality test to identify potential prime numbers.
*   Develop a client-server model to handle requests for primality testing.
*   Integrate advanced operating system mechanisms such as polling, proactors, and multi-threading to enhance server responsiveness and throughput.
*   Employ shared memory and synchronization techniques to manage state between multiple processes.

### Tasks:

#### Step A: Mathematical Foundation

*   **Objective**: Use the Miller-Rabin primality test to evaluate if a given number is a prime candidate.
*   **Details**:
    *   Accept a numeric input via `argv` and `argc`.
    *   The input number can be as large as 10^18 (type `long long`).
    *   Output whether the number is suspected to be prime based on the Miller-Rabin test.

#### Step B: Client-Server Implementation

*   **Objective**: Create a client that sends numbers to a server for primality checking.
*   **Details**:
    *   The client sends numbers ranging between 1 and 10^18.
    *   The server responds with the original number and a list of all prime candidates identified so far.
    *   Ensure repeatability by using a provided random seed for number generation.

#### Step C: Poll-Based Server for Multiple Clients

*   **Objective**: Implement a server that can handle simultaneous connections using a polling mechanism.
*   **Details**:
    *   Each received message should be logged to a file, noting the request index and any prime candidates detected.
    *   Responses should indicate whether a number is prime and, if so, the largest prime detected.
    *   Implement this using only standard C library functions for file handling (`FILE*`).

#### Step D: Proactor Pattern

*   **Objective**: Implement a general proactor pattern to handle I/O operations asynchronously.
*   **Details**:
    *   Utilize `poll` to monitor file descriptors.
    *   Spawn a thread that executes a specified function when its file descriptor is ready.
    *   Ensure thread safety for shared variables, particularly the largest prime detected and the request index.

#### Step E: Multi-Process Integration

*   **Objective**: Extend the server to use processes instead of threads for handling client requests.
*   **Details**:
    *   Use shared memory for storing the largest prime and request index, with appropriate locking mechanisms to manage access.

#### Step F: Producer-Consumer Model with Threads

*   **Objective**: Integrate a producer-consumer model to handle the accumulation of results.
*   **Details**:
    *   Implement a thread that waits for a condition (e.g., every 100 numbers checked).
    *   Upon activation, report the largest prime candidate identified so far.

#### Step G: Producer-Consumer Model with Processes

*   **Objective**: Implement the producer-consumer model using separate processes.
*   **Details**:
    *   Similar to Step F, but replace threads with processes.
    *   Ensure that processes share necessary data safely and are properly synchronized.

### Expected Outcomes:

*   Demonstrate understanding and application of complex operating system concepts.
*   Efficiently manage and synchronize multi-threaded and multi-process environments.
*   Validate and strengthen skills in system-level programming in C.

### Submission Requirements:

*   Source code for each part of the project.
*   A brief report detailing the design decisions, challenges faced, and solutions implemented.
*   Instructions for compiling and running the code.

This project will challenge your understanding of operating systems and your ability to apply theoretical concepts in practical, system-level programming scenarios.
