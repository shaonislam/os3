CC = gcc
CFLAGS = -Wall 
TARGET = oss
OBJ = main.o 

oss: $(OBJ)
	$(CC) $(CFLAGS) -o oss $(OBJ)
main.o : main.c
	$(CC) $(CFLAGS) -c main.c
clean:
	/bin/rm -f *.o $(TARGET) $(TARGET1)
