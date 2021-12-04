all: server.exe client.exe
server.exe : server.c
	gcc -o server.exe server.c -lncurses -lpthread
client.exe : client.c
	gcc -o client.exe client.c -lncurses -lpthread
clean:
	rm server.exe client.exe