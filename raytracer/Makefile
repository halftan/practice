CC = clang++
CFLAGS = -Wall -g -c -I./include -std=c++11
LDFLAGS = -L. -lfreeimage

NAME = raytracer

DEPS = main.o scene.o film.o color.o sampler.o

TEST_SCENE = scene1.test

$(NAME) : $(DEPS)
	$(CC) $(LDFLAGS) -o $(NAME) $(DEPS)

main.o : main.cpp
	$(CC) $(CFLAGS) main.cpp

scene.o : scene.cpp
	$(CC) $(CFLAGS) scene.cpp

film.o : film.cpp
	$(CC) $(CFLAGS) film.cpp

color.o : color.cpp
	$(CC) $(CFLAGS) color.cpp

sampler.o : sampler.cpp
	$(CC) $(CFLAGS) sampler.cpp

clean :
	rm *.o $(NAME)

test : $(NAME)
	./$(NAME) $(TEST_SCENE)
