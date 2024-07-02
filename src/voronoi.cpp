#include "voronoi.hpp"

#include <cassert>
#include <queue>

#include "Vector2.hpp"

inline std::int32_t get_random(std::int32_t from, std::int32_t to) {
    return rand() % (to - from) + from;
}

inline std::int32_t get_random_region(std::int32_t center,
                                      std::int32_t distance) {
    return get_random(center - distance, center + distance);
}

inline Vector2 get_random_region(Vector2 center, Vector2 distance) {
    return Vector2(get_random_region(center.x, distance.x),
                   get_random_region(center.y, distance.y));
}

std::vector<Vector2> randomizeGenerators(std::size_t N, Vector2 max) {
    std::vector<Vector2> generators;
    for (std::size_t i = 0; i < N; ++i)
        generators.push_back(Vector2(rand() % max.x, rand() % max.y));
    return generators;
}

std::vector<Vector2> rejectionSampling(std::size_t N, Image& img) {
    Grid<double> darkness(img.getHeight(), std::vector<double>(img.getWidth()));
    for (std::size_t y = 0; y < img.getHeight(); ++y)
        for (std::size_t x = 0; x < img.getWidth(); ++x)
            darkness[y][x] = Image::getDarkness(img.getColor(Vector2(x, y)));

    std::vector<Vector2> acceptedGenerators;

    Vector2 dimensions(img.getWidth(), img.getHeight());

    while (acceptedGenerators.size() < N) {
        // sample uniformly & check if their pdf is lesser than darkness.
        for (auto sample : randomizeGenerators(N - acceptedGenerators.size(), dimensions)) {
            if (rand() % 256 <= darkness[sample.y][sample.x])
                acceptedGenerators.push_back(sample);
        }
    }

    return acceptedGenerators;
}

bool operator<(const Vector2& A, const Vector2& B) {
    return A.length() < B.length();
}

Grid<std::size_t> getVoronoiDiagram(Image& img,
                                    std::vector<Vector2>& generators) {
    static Vector2 dir4[]{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    const std::uint32_t width = img.getWidth(), height = img.getHeight();

    Grid<std::size_t> voronoiImage(height, std::vector<std::size_t>(width, 0));
    Grid<bool> visited(height, std::vector<bool>(width, false));

    std::priority_queue<std::pair<std::int64_t, std::pair<Vector2, Vector2>>> Q;

    for (std::size_t i = 0; i < generators.size(); ++i) {
        Q.push({0, {generators[i], generators[i]}});
        voronoiImage[generators[i].y][generators[i].x] = i;
    }

    while (!Q.empty()) {
        auto top = Q.top();
        Q.pop();

        auto& [coord, parent] = top.second;
        if (visited[coord.y][coord.x]) continue;

        visited[coord.y][coord.x] = true;
        std::size_t index = voronoiImage[coord.y][coord.x] =
            voronoiImage[parent.y][parent.x];

        for (auto& d : dir4) {
            Vector2 child = coord + d;
            if (!child.contained(Vector2::zeroes(), Vector2(width, height)))
                continue;
            if (visited[child.y][child.x]) continue;

            Q.push({(std::int64_t)generators[index].sub(child).length() * -1,
                    {child, coord}});
        }
    }

    return voronoiImage;
}

std::vector<VoronoiBoundary> getVoronoiBoundaries(
    Image& img, std::vector<Vector2>& generators, bool drawBoundaries) {
    std::vector<VoronoiBoundary> boundaries(generators.size());

    Grid<std::size_t> voronoiImage = getVoronoiDiagram(img, generators);

    for (std::size_t y = 0; y < img.getHeight(); ++y) {
        std::size_t previousGenerator = generators.size();
        for (std::size_t x = 0; x < img.getWidth(); ++x) {
            Vector2 coord(x, y);

            std::size_t minIdx = voronoiImage[y][x];

            if (previousGenerator != minIdx) {
                boundaries[minIdx].push_back({coord, coord});

                if (drawBoundaries) img.fillPoint(coord, BLUE);

                if (previousGenerator < generators.size())
                    boundaries[previousGenerator].back().second.x = coord.x - 1;
            }

            previousGenerator = minIdx;
        }
        if (previousGenerator < generators.size())
            boundaries[previousGenerator].back().second =
                Vector2(img.getWidth() - 1, y);
    }

    return boundaries;
}

std::vector<Vector2> computeVoronoiCenters(
    std::vector<VoronoiBoundary>& boundaries,
    std::pair<PrefixFunction, PrefixFunction> prefixFunctions) {
    std::vector<Vector2> generators;

    for (auto& boundary : boundaries) {
        long double yNumerator = 0, xNumerator = 0, denominator = 0;

        for (auto& [p1, p2] : boundary) {
            assert(p1.y == p2.y);

            xNumerator += prefixFunctions.second[p2.y][p2.x] -
                          (p1.x ? prefixFunctions.second[p1.y][p1.x - 1] : 0.0);
            yNumerator +=
                p1.y * (prefixFunctions.first[p2.y][p2.x] -
                        (p1.x ? prefixFunctions.first[p1.y][p1.x - 1] : 0.0));
            denominator += prefixFunctions.first[p2.y][p2.x] -
                           (p1.x ? prefixFunctions.first[p1.y][p1.x - 1] : 0.0);
        }

        if (!(denominator > 0)) continue;

        generators.push_back(
            Vector2(xNumerator / denominator, yNumerator / denominator));
    }

    return generators;
}
