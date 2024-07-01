CC=g++
CFLAGS=-Wall -Werror -Wextra -std=c++17 -O3

all: main

main: src/main.cpp image.o src/image.hpp stb_image_write.o Vector2.o
	$(CC) $(CFLAGS) -o $@ src/main.cpp image.o stb_image_write.o Vector2.o

image.o: src/image.cpp src/image.hpp
	$(CC) $(CFLAGS) -c src/image.cpp

Vector2.o: src/Vector2.cpp src/Vector2.hpp
	$(CC) $(CFLAGS) -c src/Vector2.cpp

stb_image_write.o: src/stb_image_write.c src/stb_image_write.h
	gcc -c src/stb_image_write.c


clean:
	rm -f main.exe image.o stb_image_write.o Vector2.o
