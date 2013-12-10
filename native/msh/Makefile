NAME = msh
CC = gcc
CFLAGS = -g -c -I./include -Wall -Wno-unused-function
LDFLAGS = -o $(NAME) -lreadline
DEPS = main.o arguments.o

all : $(NAME)

$(NAME) : $(DEPS)
	$(CC) $(LDFLAGS) $(DEPS)

main.o : main.c
	$(CC) $(CFLAGS) main.c

arguments.o : arguments.c
	$(CC) $(CFLAGS) arguments.c

clean :
	rm $(DEPS) $(NAME)
