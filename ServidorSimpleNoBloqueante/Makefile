CC=gcc

compile: clean client.c server.c
	$(CC) -o client client.c
	$(CC) -o server server.c

debug: clean client.c server.c
	$(CC) -o client client.c -DDEBUG
	$(CC) -o server server.c -DDEBUG
	

clean:
	rm -f server
	rm -f client