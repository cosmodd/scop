#pragma once
#include <ostream>

class Vec2
{
	public:
		float x, y;

		Vec2();
		Vec2(float scalar);
		Vec2(float x, float y);
		Vec2(const Vec2& vec);
		Vec2& operator=(const Vec2& vec);

		Vec2 operator+(const float scalar) const;
		Vec2 operator-(const float scalar) const;
		Vec2 operator*(const float scalar) const;
		Vec2 operator/(const float scalar) const;

		Vec2 operator+(const Vec2& vec) const;
		Vec2 operator-(const Vec2& vec) const;
		Vec2 operator*(const Vec2& vec) const;

		Vec2& operator+=(const float scalar);
		Vec2& operator-=(const float scalar);
		Vec2& operator*=(const float scalar);
		Vec2& operator/=(const float scalar);

		Vec2& operator+=(const Vec2& vec);
		Vec2& operator-=(const Vec2& vec);
		Vec2& operator*=(const Vec2& vec);

		bool operator==(const Vec2& vec) const;
		bool operator!=(const Vec2& vec) const;

		float magnitude() const;
		Vec2 normalize() const;
		float dot(const Vec2& vec) const;
		float cross(const Vec2& vec) const;

		friend std::ostream& operator<<(std::ostream& os, const Vec2& vec);
};