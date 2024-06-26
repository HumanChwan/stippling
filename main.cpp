#include <iostream>

#include "image.hpp"

constexpr uint32_t FACTOR = 50;
constexpr uint32_t WIDTH  = 16 * FACTOR;
constexpr uint32_t HEIGHT = 9 * FACTOR;

int main() {
    Image img(WIDTH, HEIGHT);

    img.saveAsPNG("photo.png");
}
