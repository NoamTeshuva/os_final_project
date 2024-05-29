# chmod +x launcher.sh
# ./launcher.sh


#!/bin/bash

# Number of client instances to start
NUM_CLIENTS=300

# Server IP address and port
SERVER_IP="127.0.0.1"
SERVER_PORT=9999

# Optional: Accept server IP and port as command-line arguments
if [ "$1" != "" ]; then
    SERVER_IP=$1
fi

if [ "$2" != "" ]; then
    SERVER_PORT=$2
fi

# Compile the client program
gcc client.c -o client -lm
if [ $? -ne 0 ]; then
    echo "Error: Failed to compile the client program."
    exit 1
fi

# Start multiple client instances
for ((i=1; i<=$NUM_CLIENTS; i++)); do
    ./client $SERVER_IP $SERVER_PORT $i &
done

# Wait for all clients to finish
wait

echo "All client instances have completed."
