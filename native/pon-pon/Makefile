NAME = pon-pon
CC = gcc
CFLAGS = -Wall -g -c
LDFLAGS = -o $(NAME) -lncurses -lm

all : $(NAME)

$(NAME) : main.o
	$(CC) $(LDFLAGS) main.o

main.o : main.c main.h
	$(CC) $(CFLAGS) main.c

clean :
	rm main.o $(NAME)
