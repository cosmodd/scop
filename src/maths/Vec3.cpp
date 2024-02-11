#include <cmath>
#include "maths/Vec3.hpp"

namespace maths {

	Vec3::Vec3() : x(0.0f), y(0.0f), z(0.0f) {}

	Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

	Vec3::Vec3(const Vec3& other) : x(other.x), y(other.y), z(other.z) {}

	Vec3& Vec3::operator=(const Vec3& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	Vec3 Vec3::operator+(float scalar) const {
		return Vec3(x + scalar, y + scalar, z + scalar);
	}

	Vec3 Vec3::operator-(float scalar) const {
		return Vec3(x - scalar, y - scalar, z - scalar);
	}

	Vec3 Vec3::operator*(float scalar) const {
		return Vec3(x * scalar, y * scalar, z * scalar);
	}

	Vec3 Vec3::operator/(float scalar) const {
		return Vec3(x / scalar, y / scalar, z / scalar);
	}

	Vec3 Vec3::operator+(const Vec3& other) const {
		return Vec3(x + other.x, y + other.y, z + other.z);
	}

	Vec3 Vec3::operator-(const Vec3& other) const {
		return Vec3(x - other.x, y - other.y, z - other.z);
	}

	Vec3 Vec3::operator*(const Vec3& other) const {
		return Vec3(x * other.x, y * other.y, z * other.z);
	}

	Vec3& Vec3::operator+=(float scalar) {
		x += scalar;
		y += scalar;
		z += scalar;
		return *this;
	}

	Vec3& Vec3::operator-=(float scalar) {
		x -= scalar;
		y -= scalar;
		z -= scalar;
		return *this;
	}

	Vec3& Vec3::operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	Vec3& Vec3::operator/=(float scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	Vec3& Vec3::operator+=(const Vec3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vec3& Vec3::operator-=(const Vec3& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vec3& Vec3::operator*=(const Vec3& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	bool Vec3::operator==(const Vec3& other) const {
		return x == other.x && y == other.y && z == other.z;
	}

	bool Vec3::operator!=(const Vec3& other) const {
		return !(*this == other);
	}

	float Vec3::magnitude() const {
		return sqrt(x * x + y * y + z * z);
	}

	Vec3 Vec3::normalize() const {
		float mag = magnitude();
		return Vec3(x / mag, y / mag, z / mag);
	}

	float Vec3::dot(const Vec3& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	Vec3 Vec3::cross(const Vec3& other) const {
		return Vec3(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		);
	}

	std::ostream& operator<<(std::ostream& stream, const Vec3& vec) {
		stream << "Vec3: (" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return stream;
	}

}