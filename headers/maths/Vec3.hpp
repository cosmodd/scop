#pragma once

#include <iostream>

namespace maths {

	class Vec3 {

		public:
			float x, y, z;

			Vec3();
			Vec3(float x, float y, float z);
			Vec3(const Vec3& other);
			Vec3& operator=(const Vec3& other);

			Vec3 operator+(float scalar) const;
			Vec3 operator-(float scalar) const;
			Vec3 operator*(float scalar) const;
			Vec3 operator/(float scalar) const;

			Vec3 operator+(const Vec3& other) const;
			Vec3 operator-(const Vec3& other) const;
			Vec3 operator*(const Vec3& other) const;

			Vec3& operator+=(float scalar);
			Vec3& operator-=(float scalar);
			Vec3& operator*=(float scalar);
			Vec3& operator/=(float scalar);

			Vec3& operator+=(const Vec3& other);
			Vec3& operator-=(const Vec3& other);
			Vec3& operator*=(const Vec3& other);

			bool operator==(const Vec3& other) const;
			bool operator!=(const Vec3& other) const;

			float magnitude() const;
			Vec3 normalize() const;
			float dot(const Vec3& other) const;
			Vec3 cross(const Vec3& other) const;

	};

	std::ostream& operator<<(std::ostream& stream, const Vec3& vec);

}