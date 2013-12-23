NAME = msh
CC = gcc
CFLAGS = -g -c -I./include -Wall -Wno-unused-function
LDFLAGS = -o $(NAME) -lreadline
DEPS = main.o arguments.o exec_if.o err_handler.o util.o

all : $(NAME)

$(NAME) : $(DEPS)
	$(CC) $(LDFLAGS) $(DEPS)
	make snake
	make more

main.o : main.c
	$(CC) $(CFLAGS) main.c

arguments.o : arguments.c
	$(CC) $(CFLAGS) arguments.c

exec_if.o : exec_if.c
	$(CC) $(CFLAGS) exec_if.c

err_handler.o : err_handler.c
	$(CC) $(CFLAGS) err_handler.c

util.o : util.c
	$(CC) $(CFLAGS) util.c

clean :
	if [ -x snake ]; then rm snake; fi
	if [ -x more ]; then rm more; fi
	if [ -x $(NAME) ]; then rm $(NAME); fi
	rm *.o

snake : snake.c
	$(CC) -o snake snake.c -lncurses -pthread

more : more.c
	$(CC) -o more more.c
