
CC=clang++
CCFLAGS=-std=c++11 -c
LDFLAGS=-lm -lopencv_core -lopencv_highgui

NAME=dip4

all: ExImage.o ExHistogram.o ExFourier.o main.o
	$(CC) $(LDFLAGS) -o $(NAME) ExImage.o ExHistogram.o main.o ExFourier.o

main.o: main.cpp
	$(CC) $(CCFLAGS) main.cpp

ExImage.o: ExImage.cpp
	$(CC) $(CCFLAGS) ExImage.cpp

ExHistogram.o: ExHistogram.cpp
	$(CC) $(CCFLAGS) ExHistogram.cpp

ExFourier.o: ExFourier.cpp ExImage.cpp
	$(CC) $(CCFLAGS) ExFourier.cpp

clean:
	rm *.o $(NAME)
