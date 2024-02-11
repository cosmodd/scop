#pragma once

#include "Vec3.hpp"

namespace maths {

	// Class to represent a 4x4 matrix in row-major order
	class Mat4 {

		private:
			float elements[4 * 4];

		public:
			Mat4();
			Mat4(float diagonal);
			Mat4(const float elements[4 * 4]);
			Mat4(const Mat4& other);
			Mat4& operator=(const Mat4& other);

			float& operator()(unsigned int row, unsigned int column);
			const float& operator()(unsigned int row, unsigned int column) const;

			Mat4 operator*(const Mat4& other) const;
			Mat4 operator*(float scalar) const;
			Vec3 operator*(const Vec3& other) const;

			bool operator==(const Mat4& other) const;
			bool operator!=(const Mat4& other) const;

			Mat4 transpose() const;
			// TODO: Implement these methods later if needed
			// Mat4 inverse() const;
			// float determinant() const;

			float* getElements();
			const float* getElements() const;

			static Mat4 identity();
			static Mat4 lookAt(const Vec3& camera, const Vec3& object, const Vec3& up);
			static Mat4 orthographic(float left, float right, float bottom, float top, float near, float far);
			static Mat4 perspective(float fov, float aspectRatio, float near, float far);
			static Mat4 translation(const Vec3& translation);
			static Mat4 rotation(float angle, const Vec3& axis);
			static Mat4 scale(const Vec3& scale);

	};

}