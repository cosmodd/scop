#pragma once
#include <ostream>

class Vec3
{
	public:
		float x, y, z;

		Vec3();
		Vec3(float scalar);
		Vec3(float x, float y, float z);
		Vec3(const Vec3& vec);
		Vec3& operator=(const Vec3& vec);

		Vec3 operator+(const float scalar) const;
		Vec3 operator-(const float scalar) const;
		Vec3 operator*(const float scalar) const;
		Vec3 operator/(const float scalar) const;

		Vec3 operator+(const Vec3& vec) const;
		Vec3 operator-(const Vec3& vec) const;
		Vec3 operator*(const Vec3& vec) const;

		Vec3& operator+=(const float scalar);
		Vec3& operator-=(const float scalar);
		Vec3& operator*=(const float scalar);
		Vec3& operator/=(const float scalar);

		Vec3& operator+=(const Vec3& vec);
		Vec3& operator-=(const Vec3& vec);
		Vec3& operator*=(const Vec3& vec);

		bool operator==(const Vec3& vec) const;
		bool operator!=(const Vec3& vec) const;

		float magnitude() const;
		Vec3 normalize() const;
		float dot(const Vec3& vec) const;
		Vec3 cross(const Vec3& vec) const;

		friend std::ostream& operator<<(std::ostream& os, const Vec3& vec);
};