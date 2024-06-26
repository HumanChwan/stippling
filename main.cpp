#include <iostream>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <string>
#include <time.h>

#include "image.hpp"

constexpr uint32_t FACTOR = 100;
constexpr uint32_t WIDTH  = 16 * FACTOR;
constexpr uint32_t HEIGHT = 9 * FACTOR;

constexpr uint32_t GENERATOR_POINTS = 100;
constexpr uint32_t GENERATOR_RADIUS = 5;

constexpr uint32_t ITERATIONS = 10;

typedef std::vector<std::vector<long double>> PrefixFunction;

typedef std::vector<std::pair<Vector2, Vector2>> VoronoiBoundary;

std::vector<Vector2> generateGenerators(size_t N) {
    std::vector<Vector2> generators;
    for (size_t i = 0; i < N; ++i)
        generators.push_back(Vector2(rand() % WIDTH, rand() % HEIGHT));
    return generators;
}

std::vector<VoronoiBoundary> getVoronoiBoundaries(Image& img, std::vector<Vector2>& generators) {
    std::vector<VoronoiBoundary> boundaries(generators.size());

    for (size_t y = 0; y < img.getHeight(); ++y) {
        size_t previousGenerator = generators.size();
        for (size_t x = 0; x < img.getWidth(); ++x) {
            Vector2 coord(x, y);
            
            size_t minIdx = 0;
            for (size_t i = 0; i < generators.size(); ++i)
                if (coord.sub(generators[i]).length() < coord.sub(generators[minIdx]).length())
                    minIdx = i;

            if (previousGenerator != minIdx) {
                boundaries[minIdx].push_back({coord, coord});

                img.fillPoint(coord, BLACK);

                if (previousGenerator < generators.size())
                    boundaries[previousGenerator].back().second = coord;
            }
                
            previousGenerator = minIdx;
        }
        if (previousGenerator < generators.size())
            boundaries[previousGenerator].back().second = Vector2(img.getWidth() - 1, y);
    }

    return boundaries;
}

std::pair<PrefixFunction, PrefixFunction> computePrefixFunctions(Image& img) {
    PrefixFunction P(img.getHeight(), std::vector<long double>(img.getWidth())),
                   Q(img.getHeight(), std::vector<long double>(img.getWidth()));
    
    auto getLuminance = [](Color color) -> long double {
            uint8_t R = color >> (8 * 0),
                    G = color >> (8 * 1),
                    B = color >> (8 * 2);

            // source: https://en.wikipedia.org/wiki/Relative_luminance
            return (0.2126*R + 0.7152*G + 0.0722*B);
    };

    for (size_t y = 0; y < img.getHeight(); ++y) {

        P[y][0] = getLuminance(img.getColor(Vector2(0, y)));
        Q[y][0] = 0.0;

        for (size_t x = 1; x < img.getWidth(); ++x) {
            Vector2 coord(x, y);
            long double lum = getLuminance(img.getColor(coord));

            P[y][x] = P[y][x - 1] + lum;
            Q[y][x] = Q[y][x - 1] + x * lum;
        }
    }

    return std::make_pair(P, Q);
}

std::vector<Vector2> computeVoronoiCenters(
        std::vector<VoronoiBoundary>& boundaries, std::pair<PrefixFunction, PrefixFunction> prefixFunctions) {
    std::vector<Vector2> generators;

    for (auto& boundary : boundaries) {
        long double yNumerator = 0, 
                    xNumerator = 0,
                    denominator = 0;

        for (auto [p1, p2] : boundary) {
            assert(p1.y == p2.y);

            yNumerator += p1.y * 
                (prefixFunctions.first[p1.y][p2.x] - prefixFunctions.first[p1.y][p1.x]);

            xNumerator += prefixFunctions.second[p1.y][p2.x] - prefixFunctions.second[p1.y][p1.x];

            denominator += prefixFunctions.first[p1.y][p2.x] - prefixFunctions.first[p1.y][p1.x];
        }

        assert(denominator > 0);
        generators.push_back(Vector2(xNumerator/denominator, yNumerator/denominator));
    }
    
    return generators;
}

int main() {
    srand(time(NULL));

    Image img(WIDTH, HEIGHT);

    std::vector<Vector2> generators = generateGenerators(GENERATOR_POINTS);

    img.fillByColor(WHITE);

    std::pair<PrefixFunction, PrefixFunction> prefixFunctions = computePrefixFunctions(img);

    for (size_t i = 0; i < ITERATIONS; ++i) {
        img.fillByColor(WHITE);
        std::cout << "ITERATION: " << i + 1 << '\n';
        std::vector<VoronoiBoundary> boundaries = getVoronoiBoundaries(img, generators);
        generators = computeVoronoiCenters(boundaries, prefixFunctions);
    }

    for (auto &generator : generators)
        img.fillCircle(generator, GENERATOR_RADIUS, BLACK);

    img.saveAsPNG("photo.png");
}
