#include <cmath>

#include "maths/Mat4.hpp"

namespace maths {

	Mat4::Mat4() {
		for (unsigned int i = 0; i < 4 * 4; i++) {
			elements[i] = 0.0f;
		}
	}

	Mat4::Mat4(float diagonal) {
		for (unsigned int i = 0; i < 4 * 4; i++) {
			elements[i] = 0.0f;
		}
		elements[0 + 0 * 4] = diagonal;
		elements[1 + 1 * 4] = diagonal;
		elements[2 + 2 * 4] = diagonal;
		elements[3 + 3 * 4] = diagonal;
	}

	Mat4::Mat4(const float elements[4 * 4]) {
		for (unsigned int i = 0; i < 4 * 4; i++) {
			this->elements[i] = elements[i];
		}
	}

	Mat4::Mat4(const Mat4& other) {
		for (unsigned int i = 0; i < 4 * 4; i++) {
			elements[i] = other.elements[i];
		}
	}

	Mat4& Mat4::operator=(const Mat4& other) {
		for (unsigned int i = 0; i < 4 * 4; i++) {
			elements[i] = other.elements[i];
		}
		return *this;
	}

	float& Mat4::operator()(unsigned int row, unsigned int column) {
		return elements[column + row * 4];
	}

	const float& Mat4::operator()(unsigned int row, unsigned int column) const {
		return elements[column + row * 4];
	}

	Mat4 Mat4::operator*(const Mat4& other) const {
		Mat4 result;
		for (unsigned int row = 0; row < 4; row++) {
			for (unsigned int column = 0; column < 4; column++) {
				float sum = 0.0f;
				for (unsigned int e = 0; e < 4; e++) {
					sum += elements[e + row * 4] * other.elements[column + e * 4];
				}
				result.elements[column + row * 4] = sum;
			}
		}
		return result;
	}

	Mat4 Mat4::operator*(float scalar) const {
		Mat4 result;
		for (unsigned int i = 0; i < 4 * 4; i++) {
			result.elements[i] = elements[i] * scalar;
		}
		return result;
	}

	Vec3 Mat4::operator*(const Vec3& other) const {
		return Vec3(
			elements[0 + 0 * 4] * other.x + elements[0 + 1 * 4] * other.y + elements[0 + 2 * 4] * other.z + elements[0 + 3 * 4],
			elements[1 + 0 * 4] * other.x + elements[1 + 1 * 4] * other.y + elements[1 + 2 * 4] * other.z + elements[1 + 3 * 4],
			elements[2 + 0 * 4] * other.x + elements[2 + 1 * 4] * other.y + elements[2 + 2 * 4] * other.z + elements[2 + 3 * 4]
		);
	}

	bool Mat4::operator==(const Mat4& other) const {
		for (unsigned int i = 0; i < 4 * 4; i++) {
			if (elements[i] != other.elements[i]) {
				return false;
			}
		}
		return true;
	}

	bool Mat4::operator!=(const Mat4& other) const {
		return !(*this == other);
	}

	Mat4 Mat4::transpose() const {
		Mat4 result;
		for (unsigned int row = 0; row < 4; row++) {
			for (unsigned int column = 0; column < 4; column++) {
				result.elements[row + column * 4] = elements[column + row * 4];
			}
		}
		return result;
	}

	float* Mat4::getElements() {
		return elements;
	}

	const float* Mat4::getElements() const {
		return elements;
	}

	Mat4 Mat4::identity() {
		return Mat4(1.0f);
	}

	Mat4 Mat4::lookAt(const Vec3& camera, const Vec3& object, const Vec3& up) {
		Mat4 result;
		Vec3 forward = (object - camera).normalize();
		Vec3 right = forward.cross(up).normalize();
		Vec3 newUp = right.cross(forward);

		result(0, 0) = right.x;
		result(0, 1) = right.y;
		result(0, 2) = right.z;
		result(0, 3) = -right.dot(camera);
		result(1, 0) = newUp.x;
		result(1, 1) = newUp.y;
		result(1, 2) = newUp.z;
		result(1, 3) = -newUp.dot(camera);
		result(2, 0) = -forward.x;
		result(2, 1) = -forward.y;
		result(2, 2) = -forward.z;
		result(2, 3) = forward.dot(camera);
		result(3, 3) = 1.0f;
		return result;
	}

	Mat4 Mat4::orthographic(float left, float right, float bottom, float top, float near, float far) {
		Mat4 result(1.0f);
		result(0, 0) = 2.0f / (right - left);
		result(1, 1) = 2.0f / (top - bottom);
		result(2, 2) = 2.0f / (near - far);
		result(0, 3) = (left + right) / (left - right);
		result(1, 3) = (bottom + top) / (bottom - top);
		result(2, 3) = (far + near) / (far - near);
		return result;
	}

	Mat4 Mat4::perspective(float fov, float aspectRatio, float near, float far) {
		Mat4 result(1.0f);
		float q = 1.0f / tan(0.5f * fov);
		float a = q / aspectRatio;
		float b = (near + far) / (near - far);
		float c = (2.0f * near * far) / (near - far);
		result(0, 0) = a;
		result(1, 1) = q;
		result(2, 2) = b;
		result(2, 3) = c;
		result(3, 2) = -1.0f;
		result(3, 3) = 0.0f;
		return result;
	}

	Mat4 Mat4::translation(const Vec3& translation) {
		Mat4 result(1.0f);
		result(0, 3) = translation.x;
		result(1, 3) = translation.y;
		result(2, 3) = translation.z;
		return result;
	}

	Mat4 Mat4::rotation(float rad, const Vec3& axis) {
		Mat4 result(1.0f);
		float c = cos(rad);
		float s = sin(rad);
		float omc = 1.0f - c;
		float x = axis.x;
		float y = axis.y;
		float z = axis.z;
		result(0, 0) = x * omc + c;
		result(1, 0) = y * x * omc + z * s;
		result(2, 0) = x * z * omc - y * s;
		result(0, 1) = x * y * omc - z * s;
		result(1, 1) = y * omc + c;
		result(2, 1) = y * z * omc + x * s;
		result(0, 2) = x * z * omc + y * s;
		result(1, 2) = y * z * omc - x * s;
		result(2, 2) = z * omc + c;
		return result;
	}

	Mat4 Mat4::scale(const Vec3& scale) {
		Mat4 result(1.0f);
		result(0, 0) = scale.x;
		result(1, 1) = scale.y;
		result(2, 2) = scale.z;
		return result;
	}

}