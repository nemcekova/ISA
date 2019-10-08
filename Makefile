CC = gcc
CFLAGS = -Wall -Wextra -pedantic
run:
	$(CC) $(CFLAGS)  isaserver.c -o isaserver

clean:
	rm -f isaserver
