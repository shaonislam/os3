CC = gcc
CFLAGS = -Wall 
TARGET = oss
TARGET1 = user
OBJ = main.o 
OBJ1 = user.o
all: $(TARGET) $(TARGET1)

oss: $(OBJ)
	$(CC) $(CFLAGS) -o oss $(OBJ)
main.o : main.c
	$(CC) $(CFLAGS) -c main.c
user: $(OBJ1)
	$(CC) $(CFLAGS) -o user $(OBJ1)
user.o : user.c
	$(CC) $(CFLAGS) -c user.c
clean:
	/bin/rm -f *.o $(TARGET) $(TARGET1)
