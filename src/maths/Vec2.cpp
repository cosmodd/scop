#include "maths/Vec2.hpp"
#include <cmath>

Vec2::Vec2() : x(0.0f), y(0.0f)
{
}

Vec2::Vec2(float scalar) : x(scalar), y(scalar)
{
}

Vec2::Vec2(float x, float y) : x(x), y(y)
{
}

Vec2::Vec2(const Vec2& vec) : x(vec.x), y(vec.y)
{
}

Vec2& Vec2::operator=(const Vec2& vec)
{
	x = vec.x;
	y = vec.y;
	return *this;
}

Vec2 Vec2::operator+(const float scalar) const
{
	return Vec2(x + scalar, y + scalar);
}

Vec2 Vec2::operator-(const float scalar) const
{
	return Vec2(x - scalar, y - scalar);
}

Vec2 Vec2::operator*(const float scalar) const
{
	return Vec2(x * scalar, y * scalar);
}

Vec2 Vec2::operator/(const float scalar) const
{
	return Vec2(x / scalar, y / scalar);
}

Vec2 Vec2::operator+(const Vec2& vec) const
{
	return Vec2(x + vec.x, y + vec.y);
}

Vec2 Vec2::operator-(const Vec2& vec) const
{
	return Vec2(x - vec.x, y - vec.y);
}

Vec2 Vec2::operator*(const Vec2& vec) const
{
	return Vec2(x * vec.x, y * vec.y);
}

Vec2& Vec2::operator+=(const float scalar)
{
	x += scalar;
	y += scalar;
	return *this;
}

Vec2& Vec2::operator-=(const float scalar)
{
	x -= scalar;
	y -= scalar;
	return *this;
}

Vec2& Vec2::operator*=(const float scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}

Vec2& Vec2::operator/=(const float scalar)
{
	x /= scalar;
	y /= scalar;
	return *this;
}

Vec2& Vec2::operator+=(const Vec2& vec)
{
	x += vec.x;
	y += vec.y;
	return *this;
}

Vec2& Vec2::operator-=(const Vec2& vec)
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}

Vec2& Vec2::operator*=(const Vec2& vec)
{
	x *= vec.x;
	y *= vec.y;
	return *this;
}

bool Vec2::operator==(const Vec2& vec) const
{
	return x == vec.x && y == vec.y;
}

bool Vec2::operator!=(const Vec2& vec) const
{
	return x != vec.x || y != vec.y;
}

float Vec2::magnitude() const
{
	return std::sqrt(x * x + y * y);
}

Vec2 Vec2::normalize() const
{
	float mag = magnitude();
	return Vec2(x / mag, y / mag);
}

float Vec2::dot(const Vec2& vec) const
{
	return x * vec.x + y * vec.y;
}

float Vec2::cross(const Vec2& vec) const
{
	return x * vec.y - y * vec.x;
}

std::ostream& operator<<(std::ostream& os, const Vec2& vec)
{
	os << "Vec2(" << vec.x << ", " << vec.y << ")";
	return os;
}