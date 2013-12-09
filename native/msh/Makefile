NAME = msh
CC = gcc
CFLAGS = -g -Wall -c
LDFLAGS = -o $(NAME)

all : $(NAME)

$(NAME) : main.o
	$(CC) $(LDFLAGS) main.o

main.o : main.c main.h
	$(CC) $(CFLAGS) main.c

clean :
	rm main.o $(NAME)
