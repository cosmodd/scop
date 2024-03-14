#pragma once

#include <iostream>

namespace maths {

	class Vec2 {

		public:
			float x, y;

			Vec2();
			Vec2(float x, float y);
			Vec2(const Vec2& other);
			Vec2& operator=(const Vec2& other);

			Vec2 operator+(float scalar) const;
			Vec2 operator-(float scalar) const;
			Vec2 operator*(float scalar) const;
			Vec2 operator/(float scalar) const;

			Vec2 operator+(const Vec2& other) const;
			Vec2 operator-(const Vec2& other) const;
			Vec2 operator*(const Vec2& other) const;

			Vec2& operator+=(float scalar);
			Vec2& operator-=(float scalar);
			Vec2& operator*=(float scalar);
			Vec2& operator/=(float scalar);

			Vec2& operator+=(const Vec2& other);
			Vec2& operator-=(const Vec2& other);
			Vec2& operator*=(const Vec2& other);

			bool operator==(const Vec2& other) const;
			bool operator!=(const Vec2& other) const;

			float magnitude() const;
			Vec2 normalize() const;
			float dot(const Vec2& other) const;
			float cross(const Vec2& other) const;

	};

	std::ostream& operator<<(std::ostream& stream, const Vec2& vec);

}