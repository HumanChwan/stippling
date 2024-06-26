#include <iostream>
#include <vector>
#include <cstdlib>

#include "image.hpp"

constexpr uint32_t FACTOR = 50;
constexpr uint32_t WIDTH  = 16 * FACTOR;
constexpr uint32_t HEIGHT = 9 * FACTOR;

constexpr uint32_t GENERATOR_POINTS = 20;
constexpr uint32_t GENERATOR_RADIUS = 2;

std::vector<Vector2> generateGenerators(size_t N) {
    std::vector<Vector2> generators;
    for (size_t i = 0; i < N; ++i)
        generators.push_back(Vector2(rand() % WIDTH, rand() % HEIGHT));
    return generators;
}

void formVoronoiBoundaries(Image& img, std::vector<Vector2>& generators) {
    for (size_t y = 0; y < img.getHeight(); ++y) {
        size_t previousGenerator = generators.size();
        for (size_t x = 0; x < img.getWidth(); ++x) {
            Vector2 coord(x, y);
            
            size_t minIdx = 0;
            for (size_t i = 0; i < generators.size(); ++i)
                if (coord.sub(generators[i]).l2_distance() < coord.sub(generators[minIdx]).l2_distance())
                    minIdx = i;

            if (previousGenerator != minIdx)
                img.fillPoint(coord, BLACK);
                
            previousGenerator = minIdx;
        }
    }
}

int main() {
    Image img(WIDTH, HEIGHT);

    std::vector<Vector2> generators = generateGenerators(GENERATOR_POINTS);

    img.fillByColor(WHITE);
    formVoronoiBoundaries(img, generators);

    for (auto &generator : generators)
        img.fillCircle(generator, GENERATOR_RADIUS, RED);

    img.saveAsPNG("photo.png");
}
