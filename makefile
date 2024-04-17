CC=gcc
CFLAGS=-I. -lm

# Define the output directory for compiled binaries
BIN_DIR=bin

# Ensure the bin directory exists
$(shell mkdir -p $(BIN_DIR))

# Compile all targets
all: miller-rabin part_b_server part_b_client part_c_server part_c_client make_launcher_executable

# Compile Miller-Rabin primality test
miller-rabin: Miller-Rabin.c Miller-Rabin.h
	$(CC) -c Miller-Rabin.c -o $(BIN_DIR)/Miller-Rabin.o $(CFLAGS)

# Compile part_b server and client
part_b_server: part_b/server.c miller-rabin
	$(CC) part_b/server.c $(BIN_DIR)/Miller-Rabin.o -o $(BIN_DIR)/part_b_server $(CFLAGS)

part_b_client: part_b/client.c
	$(CC) part_b/client.c -o $(BIN_DIR)/part_b_client $(CFLAGS)

# Compile part_c server and client
part_c_server: part_c/server.c miller-rabin
	$(CC) part_c/server.c $(BIN_DIR)/Miller-Rabin.o -o $(BIN_DIR)/part_c_server $(CFLAGS)

part_c_client: part_c/client.c
	$(CC) part_c/client.c -o $(BIN_DIR)/part_c_client $(CFLAGS)

# Make launcher.sh executable
make_launcher_executable:
	chmod +x part_c/launcher.sh

# Clean the project
clean:
	rm -rf $(BIN_DIR)/*.o $(BIN_DIR)/part_b_server $(BIN_DIR)/part_b_client $(BIN_DIR)/part_c_server $(BIN_DIR)/part_c_client
