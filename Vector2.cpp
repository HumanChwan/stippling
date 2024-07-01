#include "Vector2.hpp"

#include <cmath>

Vector2::Vector2(std::int32_t x, std::int32_t y)
    : x(x), y(y) {}

Vector2 Vector2::add(std::int32_t A) {
    return Vector2(x + A, y + A);
}

Vector2 Vector2::sub(std::int32_t A) {
    return Vector2(x - A, y - A);
}

Vector2 Vector2::add(Vector2 A) {
    return Vector2(x + A.x, y + A.y);
}

Vector2 Vector2::sub(Vector2 A) {
    return Vector2(x - A.x, y - A.y);
}


Vector2 Vector2::operator+(std::int32_t A) { return add(A); }
Vector2 Vector2::operator-(std::int32_t A) { return sub(A); }

Vector2 Vector2::operator+(Vector2 A) { return add(A); }
Vector2 Vector2::operator-(Vector2 A) { return sub(A); }


std::uint64_t Vector2::l2_distance() { return 1ULL * x * x + 1ULL * y * y; }
std::uint64_t Vector2::l1_distance() { return 1LL * std::abs(x) + 1LL * std::abs(y); }
std::uint64_t Vector2::length() { return l2_distance(); }

