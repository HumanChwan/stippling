#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "Vector2.hpp"
#include "image.hpp"
#include "voronoi.hpp"

#define CONSUME(argc, argv) if (argc) argc--; argv += 1

constexpr std::uint32_t DEFAULT_GENERATOR_POINTS = 10000;
constexpr std::uint32_t DEFAULT_GENERATOR_RADIUS = 1;
constexpr std::uint32_t DEFAULT_ITERATIONS = 10;
constexpr std::uint32_t DEFAULT_SEED = 420;
constexpr const char* DEFAULT_INFILE = "./example/butterfly.png";
constexpr const char* DEFAULT_OUTFILE = "./photo.png";

class Config {
   private:
    inline static Config* instance = nullptr;
    Config() = default;
    ~Config() = default;

    std::uint32_t m_generatorPoints = DEFAULT_GENERATOR_POINTS;
    std::uint32_t m_generatorRadius = DEFAULT_GENERATOR_RADIUS;
    std::uint32_t m_iterations = DEFAULT_ITERATIONS;
    std::uint32_t m_seed = DEFAULT_SEED;
    std::string m_infilename = DEFAULT_INFILE;
    std::string m_outfilename = DEFAULT_OUTFILE;

   public:
    static Config* getInstance() {
        if (instance == nullptr) instance = new Config();
        return instance;
    }

    std::uint32_t getGeneratorPoints() const { return m_generatorPoints; }
    std::uint32_t getGeneratorRadius() const { return m_generatorRadius; }
    std::uint32_t getIterations() const { return m_iterations; }
    std::uint32_t getSeed() const { return m_seed; }
    std::string getInFilename() const { return m_infilename; }
    std::string getOutFilename() const { return m_outfilename; }

    void setGeneratorPoints(std::uint32_t x) { m_generatorPoints = x; }
    void setGeneratorRadius(std::uint32_t x) { m_generatorRadius = x; }
    void setIterations(std::uint32_t x) { m_iterations = x; }
    void setSeed(std::uint32_t x) { m_seed = x; }
    void setInFilename(std::string x) { m_infilename = x; }
    void setOutFilename(std::string x) { m_outfilename = x; }
};

void stippleAndSave(Image& img, const std::string filename) {
    const Config* config = Config::getInstance();

    std::pair<PrefixFunction, PrefixFunction> prefixFunctions =
        img.computePrefixFunctions();

    std::vector<Vector2> generators =
        rejectionSampling(config->getGeneratorPoints(), img);

    img.fillByColor(WHITE);

    for (std::size_t i = 0; i < config->getIterations(); ++i) {
        std::cout << "ITERATION: " << i + 1 << '\n';
        std::vector<VoronoiBoundary> boundaries = getVoronoiBoundaries(img, generators);
        generators = computeVoronoiCenters(boundaries, prefixFunctions);
    }

    for (auto& generator : generators)
        img.fillCircle(generator, config->getGeneratorRadius(), 0xFF181818);

    img.saveAsPNG(filename);
}

[[maybe_unused]]
void threeCircleImage() {
    static constexpr std::uint32_t FACTOR = 100;
    static constexpr std::uint32_t WIDTH = 16 * FACTOR;
    static constexpr std::uint32_t HEIGHT = 9 * FACTOR;

    Image img(WIDTH, HEIGHT);
    Vector2 dimensions(WIDTH, HEIGHT);

    img.fillByColor(WHITE);
    img.fillCircle(dimensions / 2, HEIGHT / 6, BLACK);
    img.fillCircle(dimensions / 2 - dimensions / 3, HEIGHT / 6, BLACK);
    img.fillCircle(dimensions / 2 + dimensions / 3, HEIGHT / 6, BLACK);

    stippleAndSave(img, "photo.png");
}

inline void usage() {
    std::cout << "Usage: \n" <<
                 "        $ ./stipple [-it|--iterations NUMBER] [-p|--points NUMBER]" <<
                 " [-i|--infile FILE] [-o|--outfile FILE] [-r|--radius PIXEL] [-s|--seed NUMBER]\n" << 
                 "\n" <<
                 " -it, --iterations : Number of iterations for which relaxation step takes place.\n" <<
                 "                     Default: " << DEFAULT_ITERATIONS << '\n' << 
                 " -p, --points      : Number of voronoi generator points.\n" << 
                 "                     Default: " << DEFAULT_GENERATOR_POINTS << '\n' << 
                 " -i, --infile      : Input file which is 3 or 4 component png file.\n" <<
                 "                     Default: " << DEFAULT_INFILE << '\n' << 
                 " -o, --outfile     : Output file where the output png is stored.\n" <<
                 "                     Default: " << DEFAULT_OUTFILE << '\n' << 
                 " -r, --radius      : Radius of the each generator point in pixels.\n" << 
                 "                     Default: " << DEFAULT_GENERATOR_RADIUS << '\n' << 
                 " -s, --seed        : Seed for the PRNG.\n" <<
                 "                     Default: " << DEFAULT_SEED << "\n\n";
}

std::int32_t parseInt(char* argument) {
    std::string arg = argument;
    try {
        return std::stoi(arg);
    } catch(...) {
        std::cerr << "ERROR: could not parse: '" << arg << "' to integer.\n";
        exit(1);
    }
}

void parseArguments(int argc, char** argv)  {
    CONSUME(argc, argv); // consume the executable name.

    Config* config = Config::getInstance();
    while (argc) {
        std::string argument = argv[0];
        if (argument == "-h" || argument == "--help" ) {
            usage();
            exit(0);
        } else if (argument == "-it" || argument == "--iterations") {
            CONSUME(argc, argv);
            if (!argc) { usage(); exit(1); }
            config->setIterations(parseInt(argv[0]));
        } else if (argument == "-p" || argument == "--points") {
            CONSUME(argc, argv);
            if (!argc) { usage(); exit(1); }
            config->setGeneratorPoints(parseInt(argv[0]));
        } else if (argument == "-i" || argument == "--infile") {
            CONSUME(argc, argv);
            if (!argc) { usage(); exit(1); }
            config->setInFilename(argv[0]);
        } else if (argument == "-o" || argument == "--outfile") {
            CONSUME(argc, argv);
            if (!argc) { usage(); exit(1); }
            config->setOutFilename(argv[0]);
        } else if (argument == "-r" || argument == "--radius") {
            CONSUME(argc, argv);
            if (!argc) { usage(); exit(1); }
            config->setGeneratorRadius(parseInt(argv[0]));
        } else if (argument == "-s" || argument == "--seed") {
            CONSUME(argc, argv);
            if (!argc) { usage(); exit(1); }
            config->setSeed(parseInt(argv[0]));
        }
        CONSUME(argc, argv);
    }
}

int main(int argc, char** argv) {
    parseArguments(argc, argv);

    Config* config = Config::getInstance();
    srand(config->getSeed());

    Image img = Image::from(config->getInFilename());
    stippleAndSave(img, config->getOutFilename());

    return 0;
}
