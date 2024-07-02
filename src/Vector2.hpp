#ifndef STIPPLING_VECTOR2_
#define STIPPLING_VECTOR2_

#include <cstdint>

struct Vector2 {
    std::int32_t x, y;

    static Vector2 zeroes();
    static Vector2 from(std::int32_t A);

    Vector2(std::int32_t x, std::int32_t y);

    Vector2 add(std::int32_t A) const;
    Vector2 sub(std::int32_t A) const;
    Vector2 div(std::int32_t A) const;

    Vector2 add(Vector2 A) const;
    Vector2 sub(Vector2 A) const;

    Vector2 operator+(std::int32_t A) const;
    Vector2 operator-(std::int32_t A) const;
    Vector2 operator/(std::int32_t A) const;

    Vector2 operator+(const Vector2 A) const;
    Vector2 operator-(const Vector2 A) const;

    // topLeft is inclusive, and bottomRight is exclusive
    bool contained(const Vector2 topLeft, const Vector2 bottomRight) const;

    std::uint64_t l2_distance() const;
    std::uint64_t l1_distance() const;
    std::uint64_t length() const;
};

#endif  // STIPPLING_VECTOR2_
