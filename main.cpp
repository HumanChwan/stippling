#include <iostream>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <string>
#include <time.h>

#include "image.hpp"

#include <ostream>
std::ostream& operator<<(std::ostream& os, const Vector2& v) { os << '{' << v.x << ' ' << v.y << '}'; return os; }
#include <debug.hpp>

constexpr uint32_t FACTOR = 100;
constexpr uint32_t WIDTH  = 16 * FACTOR;
constexpr uint32_t HEIGHT = 9 * FACTOR;

constexpr uint32_t GENERATOR_POINTS = 100;
constexpr uint32_t GENERATOR_RADIUS = 5;

constexpr uint32_t ITERATIONS = 10;

typedef std::vector<std::vector<long double>> PrefixFunction;

typedef std::vector<std::pair<Vector2, Vector2>> VoronoiBoundary;

inline std::int32_t get_random(std::int32_t from, std::int32_t to) {
    return rand() % (to - from) + from;
}

inline std::int32_t get_random_region(std::int32_t center, std::int32_t distance) {
    return get_random(center - distance, center + distance);
}

std::vector<Vector2> generateGenerators(std::size_t N) {
    std::vector<Vector2> generators;
    for (std::size_t i = 0; i < N; ++i)
        generators.push_back(Vector2(get_random_region(WIDTH/2, WIDTH/6), get_random_region(HEIGHT/2, HEIGHT/6)));
    return generators;

}

std::vector<VoronoiBoundary> getVoronoiBoundaries(Image& img, std::vector<Vector2>& generators) {
    std::vector<VoronoiBoundary> boundaries(generators.size());

    for (std::size_t y = 0; y < img.getHeight(); ++y) {
        std::size_t previousGenerator = generators.size();
        for (std::size_t x = 0; x < img.getWidth(); ++x) {
            Vector2 coord(x, y);
            
            std::size_t minIdx = 0;
            for (std::size_t i = 0; i < generators.size(); ++i)
                if (coord.sub(generators[i]).length() < coord.sub(generators[minIdx]).length())
                    minIdx = i;

            if (previousGenerator != minIdx) {
                boundaries[minIdx].push_back({coord, coord});

                img.fillPoint(coord, BLUE);

                if (previousGenerator < generators.size())
                    boundaries[previousGenerator].back().second.x = coord.x - 1;
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
    
    auto getDarkness = [](Color color) -> long double {
        uint32_t R = (color >> (8 * 0)) & 0xFF,
                 G = (color >> (8 * 1)) & 0xFF,
                 B = (color >> (8 * 2)) & 0xFF;

        // source: https://en.wikipedia.org/wiki/Relative_luminance
        auto darkness = 255.0 - (0.2126*R + 0.7152*G + 0.0722*B);
        return darkness * darkness * darkness;
    };

    for (std::size_t y = 0; y < img.getHeight(); ++y) {

        P[y][0] = getDarkness(img.getColor(Vector2(0, y)));
        Q[y][0] = 0.0;

        for (std::size_t x = 1; x < img.getWidth(); ++x) {
            auto darkness = getDarkness(img.getColor(Vector2(x, y)));

            P[y][x] = P[y][x - 1] + darkness;
            Q[y][x] = Q[y][x - 1] + darkness * x;
        }
    }

    return std::make_pair(P, Q);
}

std::vector<Vector2> computeVoronoiCenters(
        std::vector<VoronoiBoundary>& boundaries, std::pair<PrefixFunction, PrefixFunction> prefixFunctions) {
    std::vector<Vector2> generators;

    for (auto& boundary : boundaries) {
        // DEBUG(boundary);
        long double yNumerator = 0, 
                    xNumerator = 0,
                    denominator = 0;

        for (auto &[p1, p2] : boundary) {
            assert(p1.y == p2.y);

            xNumerator += prefixFunctions.second[p2.y][p2.x] - (p1.x ? prefixFunctions.second[p1.y][p1.x - 1] : 0.0);
            yNumerator += p1.y * 
                (prefixFunctions.first[p2.y][p2.x] - (p1.x ? prefixFunctions.first[p1.y][p1.x - 1] : 0.0));
            denominator += prefixFunctions.first[p2.y][p2.x] - (p1.x ? prefixFunctions.first[p1.y][p1.x - 1] : 0.0);
        }

        if (!(denominator > 0)) continue;

        generators.push_back(Vector2(xNumerator/denominator, yNumerator/denominator));
    }
    
    return generators;
}

int main() {
    srand(time(NULL));

    Image img(WIDTH, HEIGHT);

    std::vector<Vector2> generators = generateGenerators(GENERATOR_POINTS);

    img.fillByColor(WHITE);
    // Color colors[] = {0xFFAAAAAA, 0xFF999999, 0xFF777777, 0xFF363636, 0xFF000000};
    // for (std::size_t i = 0; i < 5; ++i)
    //     img.fillCircle(Vector2(WIDTH/2, HEIGHT/2), (5 - i) * HEIGHT/6, colors[i]);
    img.fillCircle(Vector2(WIDTH/2, HEIGHT/2), HEIGHT/6, BLACK);

    std::pair<PrefixFunction, PrefixFunction> prefixFunctions = computePrefixFunctions(img);

    for (std::size_t i = 0; i < ITERATIONS; ++i) {
        std::cout << "ITERATION: " << i + 1 << '\n';
        std::vector<VoronoiBoundary> boundaries = getVoronoiBoundaries(img, generators);
        generators = computeVoronoiCenters(boundaries, prefixFunctions);
    }

    for (auto &generator : generators)
        img.fillCircle(generator, GENERATOR_RADIUS, RED);

    img.saveAsPNG("photo.png");
}
