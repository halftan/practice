CC = g++
CFLAGS = -Wall -c -I./include -std=c++11 -O1 -g
LDFLAGS = -L. -lfreeimage -pthread

NAME = raytracer

DEPS = camera.o film.o localgeo.o\
	   main.o ray.o raytracer.o sampler.o\
	   transform.o scene.o solve.o sphere.o\
	   tri.o vertex.o light.o point_light.o\
	   direction_light.o

TEST_SCENE1 = scene1.test
TEST_SCENE2 = scene2.test
TEST_SCENE3 = scene3.test

$(NAME) : $(DEPS)
	$(CC) $(LDFLAGS) -o $(NAME) $(DEPS)

camera.o : camera.cpp
	$(CC) $(CFLAGS) camera.cpp

# color.o : color.cpp
# 	$(CC) $(CFLAGS) color.cpp

film.o : film.cpp
	$(CC) $(CFLAGS) film.cpp

localgeo.o : localgeo.cpp
	$(CC) $(CFLAGS) localgeo.cpp

main.o : main.cpp
	$(CC) $(CFLAGS) main.cpp

ray.o : ray.cpp
	$(CC) $(CFLAGS) ray.cpp

raytracer.o : raytracer.cpp
	$(CC) $(CFLAGS) raytracer.cpp

sampler.o : sampler.cpp
	$(CC) $(CFLAGS) sampler.cpp

scene.o : scene.cpp
	$(CC) $(CFLAGS) scene.cpp

solve.o : solve.cpp
	$(CC) $(CFLAGS) solve.cpp

sphere.o : sphere.cpp
	$(CC) $(CFLAGS) sphere.cpp

transform.o : transform.cpp
	$(CC) $(CFLAGS) transform.cpp

tri.o : tri.cpp
	$(CC) $(CFLAGS) tri.cpp

vertex.o : vertex.cpp
	$(CC) $(CFLAGS) vertex.cpp

light.o : light.cpp
	$(CC) $(CFLAGS) light.cpp

point_light.o : point_light.cpp
	$(CC) $(CFLAGS) point_light.cpp

direction_light.o : direction_light.cpp
	$(CC) $(CFLAGS) direction_light.cpp

clean :
	rm *.o $(NAME)

remake : clean $(NAME)

test1 : $(NAME)
	./$(NAME) $(TEST_SCENE1)

test2 : $(NAME)
	./$(NAME) $(TEST_SCENE2)

test3 : $(NAME)
	./$(NAME) $(TEST_SCENE3)
