#include <vector>
#include <string> 
#include <cstdint>

typedef std::uint32_t Color;
typedef std::vector<Color> PixelMap;

#define RED   ((Color)0xFF0000FF)
#define GREEN ((Color)0xFF00FF00)
#define BLUE  ((Color)0xFFFF00FF)
#define BLACK ((Color)0xFF000000)

class Image {
private:
    PixelMap data; 
    size_t width, height, stride;
public:
    Image(size_t width, size_t height);

    void fillByColor(Color color);

    // Methods to save images to disk
    void saveAsPNG(const std::string filename);
    void saveAsPPM(const std::string filename);
};
