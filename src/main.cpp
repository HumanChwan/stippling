#include <time.h>

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <queue>
#include <string>
#include <vector>

#include "Vector2.hpp"
#include "image.hpp"
#include "voronoi.hpp"

constexpr uint32_t FACTOR = 100;
constexpr uint32_t WIDTH = 16 * FACTOR;
constexpr uint32_t HEIGHT = 9 * FACTOR;

constexpr uint32_t GENERATOR_POINTS = 2000;
constexpr uint32_t GENERATOR_RADIUS = 5;

constexpr uint32_t ITERATIONS = 10;


int main() {
    srand(time(NULL));

    Image img(WIDTH, HEIGHT);

    Vector2 dimensions(WIDTH, HEIGHT);

    std::vector<Vector2> generators = randomizeGenerators(GENERATOR_POINTS, dimensions);

    img.fillByColor(0xFFFEFEFE);
    img.fillCircle(dimensions / 2, HEIGHT / 6, BLACK);

    std::pair<PrefixFunction, PrefixFunction> prefixFunctions =
        img.computePrefixFunctions();

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
