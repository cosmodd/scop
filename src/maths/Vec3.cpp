#include "maths/Vec3.hpp"
#include <cmath>

Vec3::Vec3() : x(0.0f), y(0.0f), z(0.0f)
{
}

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z)
{
}

Vec3::Vec3(const Vec3 &vec) : x(vec.x), y(vec.y), z(vec.z)
{
}

Vec3 &Vec3::operator=(const Vec3 &vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}

Vec3 Vec3::operator+(const float scalar) const
{
	return Vec3(x + scalar, y + scalar, z + scalar);
}

Vec3 Vec3::operator-(const float scalar) const
{
	return Vec3(x - scalar, y - scalar, z - scalar);
}

Vec3 Vec3::operator*(const float scalar) const
{
	return Vec3(x * scalar, y * scalar, z * scalar);
}

Vec3 Vec3::operator/(const float scalar) const
{
	return Vec3(x / scalar, y / scalar, z / scalar);
}

Vec3 Vec3::operator+(const Vec3 &vec) const
{
	return Vec3(x + vec.x, y + vec.y, z + vec.z);
}

Vec3 Vec3::operator-(const Vec3 &vec) const
{
	return Vec3(x - vec.x, y - vec.y, z - vec.z);
}

Vec3 Vec3::operator*(const Vec3 &vec) const
{
	return Vec3(x * vec.x, y * vec.y, z * vec.z);
}

Vec3 &Vec3::operator+=(const float scalar)
{
	x += scalar;
	y += scalar;
	z += scalar;
	return *this;
}

Vec3 &Vec3::operator-=(const float scalar)
{
	x -= scalar;
	y -= scalar;
	z -= scalar;
	return *this;
}

Vec3 &Vec3::operator*=(const float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

Vec3 &Vec3::operator/=(const float scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

Vec3 &Vec3::operator+=(const Vec3 &vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

Vec3 &Vec3::operator-=(const Vec3 &vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

Vec3 &Vec3::operator*=(const Vec3 &vec)
{
	x *= vec.x;
	y *= vec.y;
	z *= vec.z;
	return *this;
}

bool Vec3::operator==(const Vec3 &vec) const
{
	return x == vec.x && y == vec.y && z == vec.z;
}

bool Vec3::operator!=(const Vec3 &vec) const
{
	return !(*this == vec);
}

float Vec3::magnitude() const
{
	return sqrt(x * x + y * y + z * z);
}

Vec3 Vec3::normalize() const
{
	float mag = magnitude();
	return Vec3(x / mag, y / mag, z / mag);
}

float Vec3::dot(const Vec3 &vec) const
{
	return x * vec.x + y * vec.y + z * vec.z;
}

Vec3 Vec3::cross(const Vec3 &vec) const
{
	return Vec3(
		y * vec.z - z * vec.y,
		z * vec.x - x * vec.z,
		x * vec.y - y * vec.x);
}

std::ostream &operator<<(std::ostream &os, const Vec3 &vec)
{
	os << "Vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}