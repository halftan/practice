
CC=clang++
CCFLAGS=-std=c++11 -c
LDFLAGS=-lm -lopencv_core -lopencv_highgui

NAME=dip3

all: ExImage.o ExHistogram.o main.o
	$(CC) $(LDFLAGS) -o $(NAME) ExImage.o ExHistogram.o main.o

main.o: main.cpp
	$(CC) $(CCFLAGS) main.cpp

ExImage.o: ExImage.cpp
	$(CC) $(CCFLAGS) ExImage.cpp

ExHistogram.o: ExHistogram.cpp
	$(CC) $(CCFLAGS) ExHistogram.cpp

clean:
	rm main.o ExImage.o ExHistogram.o $(NAME)
