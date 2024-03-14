#include <cmath>
#include "maths/Vec2.hpp"

namespace maths {

	Vec2::Vec2(): x(0), y(0) {}

	Vec2::Vec2(float x, float y): x(x), y(y) {}

	Vec2::Vec2(const Vec2& other): x(other.x), y(other.y) {}

	Vec2& Vec2::operator=(const Vec2& other) {
		x = other.x;
		y = other.y;
		return *this;
	}

	Vec2 Vec2::operator+(float scalar) const {
		return Vec2(x + scalar, y + scalar);
	}

	Vec2 Vec2::operator-(float scalar) const {
		return Vec2(x - scalar, y - scalar);
	}

	Vec2 Vec2::operator*(float scalar) const {
		return Vec2(x * scalar, y * scalar);
	}

	Vec2 Vec2::operator/(float scalar) const {
		return Vec2(x / scalar, y / scalar);
	}

	Vec2 Vec2::operator+(const Vec2& other) const {
		return Vec2(x + other.x, y + other.y);
	}

	Vec2 Vec2::operator-(const Vec2& other) const {
		return Vec2(x - other.x, y - other.y);
	}

	Vec2 Vec2::operator*(const Vec2& other) const {
		return Vec2(x * other.x, y * other.y);
	}

	Vec2& Vec2::operator+=(float scalar) {
		x += scalar;
		y += scalar;
		return *this;
	}

	Vec2& Vec2::operator-=(float scalar) {
		x -= scalar;
		y -= scalar;
		return *this;
	}

	Vec2& Vec2::operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vec2& Vec2::operator/=(float scalar) {
		x /= scalar;
		y /= scalar;
		return *this;
	}

	Vec2& Vec2::operator+=(const Vec2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	Vec2& Vec2::operator-=(const Vec2& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vec2& Vec2::operator*=(const Vec2& other) {
		x *= other.x;
		y *= other.y;
		return *this;
	}

	bool Vec2::operator==(const Vec2& other) const {
		return x == other.x && y == other.y;
	}

	bool Vec2::operator!=(const Vec2& other) const {
		return !(*this == other);
	}

	float Vec2::magnitude() const {
		return std::sqrt(x * x + y * y);
	}

	Vec2 Vec2::normalize() const {
		float mag = magnitude();
		return Vec2(x / mag, y / mag);
	}

	float Vec2::dot(const Vec2& other) const {
		return x * other.x + y * other.y;
	}

	float Vec2::cross(const Vec2& other) const {
		return x * other.y - y * other.x;
	}

	std::ostream& operator<<(std::ostream& stream, const Vec2& vec) {
		stream << "Vec2(" << vec.x << ", " << vec.y << ")";
		return stream;
	}

}