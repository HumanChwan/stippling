#ifndef STIPPLING_VORONOI_
#define STIPPLING_VORONOI_

#include <cstdint>
#include <vector>

#include "Vector2.hpp"
#include "image.hpp"

template <typename T>
using Grid = std::vector<std::vector<T>>;

typedef std::vector<std::pair<Vector2, Vector2>> VoronoiBoundary;

std::vector<Vector2> randomizeGenerators(std::size_t N, Vector2 max);
std::vector<Vector2> rejectionSampling(std::size_t N, Image& img);

Grid<std::size_t> getVoronoiDiagram(Image& img,
                                    std::vector<Vector2>& generators);

std::vector<VoronoiBoundary> getVoronoiBoundaries(
    Image& img, std::vector<Vector2>& generators, bool drawBoundaries = false);

std::vector<Vector2> computeVoronoiCenters(
    std::vector<VoronoiBoundary>& boundaries,
    std::pair<PrefixFunction, PrefixFunction> prefixFunctions);

#endif  // STIPPLING_VORONOI_
