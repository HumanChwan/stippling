#include <vector>
#include <string> 
#include <cstdint>

#include "Vector2.hpp"

typedef std::uint32_t Color;
typedef std::vector<Color> PixelMap;

#define RED   ((Color)0xFF0000FF)
#define GREEN ((Color)0xFF00FF00)
#define BLUE  ((Color)0xFFFF0000)
#define BLACK ((Color)0xFF000000)
#define WHITE ((Color)0xFFFFFFFF)

class Image {
private:
    PixelMap data; 
    size_t width, height, stride;

    bool isValidVector(Vector2 coord);
public:
    Image(size_t width, size_t height);

    size_t getWidth() const;
    size_t getHeight() const;

    Color getColor(Vector2 coord);

    void fillPoint(Vector2 coord, Color color) { data[coord.y * stride + coord.x] = color; }
    void fillByColor(Color color);
    void fillCircle(Vector2 center, size_t radius, Color color);
    void fillRectangle(Vector2 topLeft, size_t width, size_t height, Color color);

    // Methods to save images to disk
    void saveAsPNG(const std::string filename) const;
    void saveAsPPM(const std::string filename) const;
};
