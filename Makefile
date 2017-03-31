.SUFFIXES:
.SUFFIXES: .c .o

CC = gcc

BINS=robotClient robotServer

all:	$(BINS)

robotClient:	UDPClient.c
	$(CC) $(CFLAGS) -o robotClient UDPClient.c

robotServer:	middleware.c
	$(CC) $(CFLAGS) -o robotServer middleware.c DieWithError.c

clean:
	rm -f *.o *.ppm *.~ dnsq

