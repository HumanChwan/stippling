#include <algorithm>

#include "image.hpp"

#include "stb_image_write.h"

bool Image::isValidVector(Vector2 vec) {
    return (0 <= vec.x && vec.x < (int32_t)width && 0 <= vec.y && vec.y < (int32_t)height);
}

size_t Image::getWidth() const { return width; }
size_t Image::getHeight() const { return height; }

Image::Image(size_t width, size_t height)
    : width(width), height(height), stride(width) {
    data.assign(height * width, BLACK);
}

void Image::fillByColor(Color color) {
    for (size_t y = 0; y < height; ++y)
        for (size_t x = 0; x < width; ++x)
            fillPoint(Vector2(x, y), color);
}

void Image::fillCircle(Vector2 center, size_t radius, Color color) {
    Vector2 topLeft = center.sub(radius),
            bottomRight = center.add(radius);
    for (int32_t y = topLeft.y; y <= bottomRight.y; ++y) {
        for (int32_t x = topLeft.x; x <= bottomRight.x; ++x) {
            Vector2 vec(x, y);
            if (!isValidVector(vec)) continue;

            uint64_t distance = vec.sub(center).l2_distance();

            if (distance <= 1ULL * radius * radius) 
                fillPoint(vec, color);
        }
    }
}

void Image::saveAsPNG(const std::string filename) const {
     stbi_write_png(filename.c_str(), width, height, 4, data.data(), stride * sizeof(Color));
}

void Image::saveAsPPM(const std::string filename) const {
    (void)filename;
    throw "Not Implemented.";
}
