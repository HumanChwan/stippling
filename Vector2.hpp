#include <cstdint>

struct Vector2 {
    std::int32_t x, y;

    Vector2(std::int32_t x, std::int32_t y);

    Vector2 add(std::int32_t A);
    Vector2 sub(std::int32_t A);

    Vector2 add(Vector2 A);
    Vector2 sub(Vector2 A);

    Vector2 operator+(std::int32_t A);
    Vector2 operator-(std::int32_t A);

    Vector2 operator+(Vector2 A);
    Vector2 operator-(Vector2 A);

    std::uint64_t l2_distance();
    std::uint64_t l1_distance();
    std::uint64_t length();
};

