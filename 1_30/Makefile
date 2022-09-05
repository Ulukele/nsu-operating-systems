all: server client

server: server.c
	gcc server.c -lsocket -lnsl -o server
client: client.c
	gcc client.c -lsocket -lnsl -o client
clean:
	rm -f server client