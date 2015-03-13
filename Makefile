
CC = gcc
CFLAGS = -g -Wall
OBJS_CLI = cli.o client.o
OBJS_SERV = serv.o server.o
SRC_CLI = cli.c client.c
SRC_SERV = serv.c server.c

all : Cli_Serv

Cli_Serv : $(OBJS_CLI) $(OBJS_SERV)
	$(CC) $(CFLAGS) -o client $(OBJS_CLI) -lpthread
	$(CC) $(CFLAGS) -o server $(OBJS_SERV) -lpthread

Cli : $(OBJS_CLI) 
	$(CC) $(CFLAGS) -o client $(OBJS_CLI) -lpthread

Serv : $(OBJS_SERV)
	$(CC) $(CFLAGS) -o server $(OBJS_SERV) -lpthread

$(OBJS_CLI) : $(SRC_CLI)
	$(CC) $(CFLAGS) -c $(SRC_CLI)

$(OBJS_SERV) : $(SRC_SERV)
	$(CC) $(CFLAGS) -c $(SRC_SERV)

clean :
	rm $(OBJS_CLI) $(OBJS_SERV)

remove:
	rm client 
	rm server

