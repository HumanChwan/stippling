CC=g++
CFLAGS=-Wall -Werror -Wextra -std=c++17 -O3

all: main

main: main.cpp image.o image.hpp stb_image_write.o Vector2.o
	$(CC) $(CFLAGS) -o $@ main.cpp image.o stb_image_write.o Vector2.o

image.o: image.cpp image.hpp
	$(CC) $(CFLAGS) -c image.cpp

Vector2.o: Vector2.cpp Vector2.hpp
	$(CC) $(CFLAGS) -c Vector2.cpp

stb_image_write.o: stb_image_write.c stb_image_write.h
	gcc -c stb_image_write.c


clean:
	rm -f main.exe image.o stb_image_write.o Vector2.o
