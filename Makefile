CC=g++
CFLAGS=-Wall -Werror -Wextra -std=c++17 -O3 -g
OBJECT_FILES=image.o Vector2.o voronoi.o stb_image_write.o stb_image.o
HEADER_FILES=src/image.hpp src/Vector2.hpp src/voronoi.hpp src/thirdparty/stb_image_write.h src/thirdparty/stb_image.h

all: stipple

stipple: src/main.cpp $(OBJECT_FILES) $(HEADER_FILES)
	$(CC) $(CFLAGS) -o $@ src/main.cpp $(OBJECT_FILES)

image.o: src/image.cpp src/image.hpp
	$(CC) $(CFLAGS) -c src/image.cpp

Vector2.o: src/Vector2.cpp src/Vector2.hpp
	$(CC) $(CFLAGS) -c src/Vector2.cpp

voronoi.o: src/voronoi.cpp src/voronoi.hpp
	$(CC) $(CFLAGS) -c src/voronoi.cpp

stb_image_write.o: src/thirdparty/stb_image_write.c src/thirdparty/stb_image_write.h
	gcc -c src/thirdparty/stb_image_write.c

stb_image.o: src/thirdparty/stb_image.c src/thirdparty/stb_image.h
	gcc -c src/thirdparty/stb_image.c


clean:
	rm -f stipple $(OBJECT_FILES)
