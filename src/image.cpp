#include "image.hpp"

#include <algorithm>

#include "thirdparty/stb_image_write.h"

bool Image::isValidVector(Vector2 vec) {
    return (0 <= vec.x && vec.x < (int32_t)width && 0 <= vec.y &&
            vec.y < (int32_t)height);
}

size_t Image::getWidth() const { return width; }
size_t Image::getHeight() const { return height; }

Color Image::getColor(Vector2 coord) {
    return data[coord.y * stride + coord.x];
}

Image::Image(size_t width, size_t height)
    : width(width), height(height), stride(width) {
    data.assign(height * width, BLACK);
}

void Image::fillByColor(Color color) {
    for (size_t y = 0; y < height; ++y)
        for (size_t x = 0; x < width; ++x) fillPoint(Vector2(x, y), color);
}

void Image::fillCircle(Vector2 center, size_t radius, Color color) {
    Vector2 topLeft = center.sub(radius), bottomRight = center.add(radius);
    for (int32_t y = topLeft.y; y <= bottomRight.y; ++y) {
        for (int32_t x = topLeft.x; x <= bottomRight.x; ++x) {
            Vector2 vec(x, y);
            if (!isValidVector(vec)) continue;

            uint64_t distance = vec.sub(center).l2_distance();

            if (distance <= 1ULL * radius * radius) fillPoint(vec, color);
        }
    }
}

void Image::fillRectangle(Vector2 topLeft, size_t r_width, size_t r_height,
                          Color color) {
    for (size_t y = topLeft.y; y <= topLeft.y + r_height; ++y) {
        for (size_t x = topLeft.x; x <= topLeft.x + r_width; ++x) {
            Vector2 coord(x, y);
            if (!isValidVector(coord)) continue;
            fillPoint(coord, color);
        }
    }
}

double Image::getDarkness(Color color) {
    uint32_t R = (color >> (8 * 0)) & 0xFF, G = (color >> (8 * 1)) & 0xFF,
             B = (color >> (8 * 2)) & 0xFF;

    // source: https://en.wikipedia.org/wiki/Relative_luminance
    auto darkness = 256.0 - (0.2126 * R + 0.7152 * G + 0.0722 * B);
    return darkness * darkness *
           darkness;  // increase the ``weight" of darkness by cubing.
}

std::pair<PrefixFunction, PrefixFunction> Image::computePrefixFunctions() {
    PrefixFunction P(getHeight(), std::vector<long double>(getWidth())),
        Q(getHeight(), std::vector<long double>(getWidth()));

    for (std::size_t y = 0; y < getHeight(); ++y) {
        P[y][0] = Image::getDarkness(getColor(Vector2(0, y)));
        Q[y][0] = 0.0;

        for (std::size_t x = 1; x < getWidth(); ++x) {
            auto darkness = Image::getDarkness(getColor(Vector2(x, y)));

            P[y][x] = P[y][x - 1] + darkness;
            Q[y][x] = Q[y][x - 1] + darkness * x;
        }
    }

    return std::make_pair(P, Q);
}

void Image::saveAsPNG(const std::string filename) const {
    stbi_write_png(filename.c_str(), width, height, 4, data.data(),
                   stride * sizeof(Color));
}

void Image::saveAsPPM(const std::string filename) const {
    (void)filename;
    throw "Not Implemented.";
}
