CC = gcc
CFLAGS = -pthread
TARGET = server

OBJS = main.o server.o commands.o user.o contact.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

main.o: main.c server.h
	$(CC) -c main.c

server.o: server.c server.h commands.h
	$(CC) -c server.c

commands.o: commands.c commands.h user.h contact.h
	$(CC) -c commands.c

users.o: user.c user.h
	$(CC) -c users.c

contacts.o: contact.c contact.h
	$(CC) -c contacts.c

clean:
	rm -f $(TARGET) $(OBJS)
