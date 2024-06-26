#include <algorithm>

#include "image.hpp"

#include "stb_image_write.h"

Image::Image(size_t width, size_t height)
    : width(width), height(height), stride(width) {
    data.assign(height * width, BLACK);
}

void Image::fillByColor(Color color) {
    for (size_t y = 0; y < height; ++y)
        for (size_t x = 0; x < width; ++x)
            data[y * stride + x] = color;
}

void Image::saveAsPNG(const std::string filename) {
     stbi_write_png(filename.c_str(), width, height, 4, data.data(), stride * sizeof(Color));
}

void Image::saveAsPPM(const std::string filename) {
    (void)filename;
    throw "Not Implemented.";
}
