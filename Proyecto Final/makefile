CC = gcc
CFLAGS = -Wall -std=c99
TARGETS = publicador suscriptor sistema

all: $(TARGETS)

publicador: publicador.c
	$(CC) $(CFLAGS) -o publicador publicador.c

suscriptor: suscriptor.c
	$(CC) $(CFLAGS) -o suscriptor suscriptor.c

sistema: sistema.c
	$(CC) $(CFLAGS) -o sistema sistema.c

clean:
	rm -f $(TARGETS) *.o

