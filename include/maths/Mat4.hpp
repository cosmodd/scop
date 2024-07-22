#pragma once
#include <ostream>
#include "Vec3.hpp"

class Mat4
{

	private:
		float elements[4 * 4];

	public:
		Mat4();
		Mat4(const float diagonal);
		Mat4(const float elements[4 * 4]);
		Mat4(const Mat4& mat);
		Mat4& operator=(const Mat4& mat);

		float& operator()(const unsigned int row, const unsigned int column);
		const float& operator()(const unsigned int row, const unsigned int column) const;

		Mat4 operator+(const Mat4& mat) const;
		Mat4 operator-(const Mat4& mat) const;

		Mat4 operator*(const Mat4& mat) const;
		Mat4 operator*(const float scalar) const;
		Vec3 operator*(const Vec3& vec) const;

		Mat4 operator+=(const Mat4& mat);
		Mat4 operator-=(const Mat4& mat);
		Mat4 operator*=(const Mat4& mat);

		bool operator==(const Mat4& mat) const;
		bool operator!=(const Mat4& mat) const;

		const float* getElements() const;

		Mat4 transpose() const;

		static Mat4 identity();
		static Mat4 lookAt(const Vec3& position, const Vec3& target, const Vec3& up);
		static Mat4 orthographic(const float left, const float right, const float bottom, const float top, const float near, const float far);
		static Mat4 perspective(const float fov, const float aspectRatio, const float near, const float far);
		static Mat4 translation(const Vec3& translation);
		static Mat4 rotation(const float rad, const Vec3& axis);
		static Mat4 scale(const Vec3& scale);

		friend std::ostream& operator<<(std::ostream& os, const Mat4& mat);

};