#include <cstdint>

struct Vector2 {
    int32_t x, y;

    Vector2(int32_t x, int32_t y);

    Vector2 add(int32_t A);
    Vector2 sub(int32_t A);

    Vector2 add(Vector2 A);
    Vector2 sub(Vector2 A);

    Vector2 operator+(int32_t A);
    Vector2 operator-(int32_t A);

    Vector2 operator+(Vector2 A);
    Vector2 operator-(Vector2 A);

    uint64_t l2_distance();
    uint64_t l1_distance();
    uint64_t length();
};


