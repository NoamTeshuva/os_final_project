CC = gcc
CFLAGS = -Wall -Wextra -pthread

.PHONY: all clean

all: serverCond client

serverCond: server_with_cond.c ../Miller-Rabin.c ../part_d/proactor.c
	$(CC) $(CFLAGS) -o serverCond server_with_cond.c ../Miller-Rabin.c ../part_d/proactor.c -lpthread

client: client.c
	$(CC) $(CFLAGS) -o client client.c

clean:
	rm -f serverCond client
