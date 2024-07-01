#include "Vector2.hpp"

#include <cmath>

Vector2::Vector2(std::int32_t x, std::int32_t y) : x(x), y(y) {}

Vector2 Vector2::add(std::int32_t A) const { return Vector2(x + A, y + A); }

Vector2 Vector2::sub(std::int32_t A) const { return Vector2(x - A, y - A); }

Vector2 Vector2::add(Vector2 A) const { return Vector2(x + A.x, y + A.y); }

Vector2 Vector2::sub(Vector2 A) const { return Vector2(x - A.x, y - A.y); }

Vector2 Vector2::operator+(std::int32_t A) const { return add(A); }
Vector2 Vector2::operator-(std::int32_t A) const { return sub(A); }

Vector2 Vector2::operator+(const Vector2 A) const { return add(A); }
Vector2 Vector2::operator-(const Vector2 A) const { return sub(A); }

// bool operator<(const Vector2& A, const Vector2& B) {
//     return A.length() < B.length();
// }

bool Vector2::contained(const Vector2 topLeft,
                        const Vector2 bottomRight) const {
    return (topLeft.x <= x && x < bottomRight.x && topLeft.y <= y &&
            y < bottomRight.y);
}

std::uint64_t Vector2::l2_distance() const {
    return 1ULL * x * x + 1ULL * y * y;
}
std::uint64_t Vector2::l1_distance() const {
    return 1LL * std::abs(x) + 1LL * std::abs(y);
}
std::uint64_t Vector2::length() const { return l2_distance(); }
