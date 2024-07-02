CC=g++
CFLAGS=-Wall -Werror -Wextra -std=c++17 -O3
OBJECT_FILES=image.o stb_image_write.o Vector2.o voronoi.o
HEADER_FILES=src/image.hpp src/Vector2.hpp src/voronoi.hpp src/stb_image_write.h

all: main

main: src/main.cpp $(OBJECT_FILES) $(HEADER_FILES)
	$(CC) $(CFLAGS) -o $@ src/main.cpp $(OBJECT_FILES)

image.o: src/image.cpp src/image.hpp
	$(CC) $(CFLAGS) -c src/image.cpp

Vector2.o: src/Vector2.cpp src/Vector2.hpp
	$(CC) $(CFLAGS) -c src/Vector2.cpp

voronoi.o: src/voronoi.cpp src/voronoi.hpp
	$(CC) $(CFLAGS) -c src/voronoi.cpp

stb_image_write.o: src/stb_image_write.c src/stb_image_write.h
	gcc -c src/stb_image_write.c


clean:
	rm -f main.exe $(OBJECT_FILES)
