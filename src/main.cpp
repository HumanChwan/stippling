#include <time.h>

#include <iostream>
#include <vector>
#include <cstdlib>

#include "Vector2.hpp"
#include "image.hpp"
#include "voronoi.hpp"

constexpr std::uint32_t FACTOR = 100;
constexpr std::uint32_t WIDTH = 16 * FACTOR;
constexpr std::uint32_t HEIGHT = 9 * FACTOR;

constexpr std::uint32_t GENERATOR_POINTS = 200;
constexpr std::uint32_t GENERATOR_RADIUS = 5;

constexpr std::uint32_t ITERATIONS = 10;

int main() {
    srand(time(NULL));

    Image img(WIDTH, HEIGHT);
    Vector2 dimensions(WIDTH, HEIGHT);

    img.fillByColor(WHITE);
    img.fillCircle(dimensions / 2, HEIGHT / 6, BLACK);
    img.fillCircle(dimensions / 2 - dimensions / 3, HEIGHT / 6, BLACK);
    img.fillCircle(dimensions / 2 + dimensions / 3, HEIGHT / 6, BLACK);

    std::pair<PrefixFunction, PrefixFunction> prefixFunctions =
        img.computePrefixFunctions();

    std::vector<Vector2> generators =
        rejectionSampling(GENERATOR_POINTS, img);

    for (std::size_t i = 0; i < ITERATIONS; ++i) {
        std::cout << "ITERATION: " << i + 1 << '\n';
        std::vector<VoronoiBoundary> boundaries =
            getVoronoiBoundaries(img, generators, (i == ITERATIONS - 1));
        generators = computeVoronoiCenters(boundaries, prefixFunctions);
    }

    for (auto& generator : generators)
        img.fillCircle(generator, GENERATOR_RADIUS, RED);

    img.saveAsPNG("photo.png");
}
