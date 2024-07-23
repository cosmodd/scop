#include "maths/Mat4.hpp"
#include <cmath>
#include <iomanip>
#include <string>
#include <iostream>

Mat4::Mat4()
{
	for (unsigned int i = 0; i < 4 * 4; i++)
	{
		elements[i] = 0.0f;
	}
}

Mat4::Mat4(const float diagonal)
{
	for (unsigned int i = 0; i < 4 * 4; i++)
	{
		elements[i] = 0.0f;
	}
	elements[0 + 0 * 4] = diagonal;
	elements[1 + 1 * 4] = diagonal;
	elements[2 + 2 * 4] = diagonal;
	elements[3 + 3 * 4] = diagonal;
}

Mat4::Mat4(const float elements[4 * 4])
{
	for (unsigned int i = 0; i < 4 * 4; i++)
	{
		this->elements[i] = elements[i];
	}
}

Mat4::Mat4(const Mat4& mat)
{
	for (unsigned int i = 0; i < 4 * 4; i++)
	{
		elements[i] = mat.elements[i];
	}
}

Mat4& Mat4::operator=(const Mat4& mat)
{
	for (unsigned int i = 0; i < 4 * 4; i++)
	{
		elements[i] = mat.elements[i];
	}
	return *this;
}

float& Mat4::operator()(const unsigned int row, const unsigned int column)
{
	return elements[column + row * 4];
}

const float& Mat4::operator()(const unsigned int row, const unsigned int column) const
{
	return elements[column + row * 4];
}

Mat4 Mat4::operator+(const Mat4& mat) const
{
	Mat4 result;
	for (unsigned int i = 0; i < 4 * 4; i++)
	{
		result.elements[i] = elements[i] + mat.elements[i];
	}
	return result;
}

Mat4 Mat4::operator-(const Mat4& mat) const
{
	Mat4 result;
	for (unsigned int i = 0; i < 4 * 4; i++)
	{
		result.elements[i] = elements[i] - mat.elements[i];
	}
	return result;
}

Mat4 Mat4::operator*(const Mat4& mat) const
{
	Mat4 result;
	for (unsigned int row = 0; row < 4; row++)
	{
		for (unsigned int column = 0; column < 4; column++)
		{
			float sum = 0.0f;
			for (unsigned int i = 0; i < 4; i++)
			{
				sum += elements[i + row * 4] * mat.elements[column + i * 4];
			}
			result.elements[column + row * 4] = sum;
		}
	}
	return result;
}

Mat4 Mat4::operator*(const float scalar) const
{
	Mat4 result;
	for (unsigned int i = 0; i < 4 * 4; i++)
	{
		result.elements[i] = elements[i] * scalar;
	}
	return result;
}

Vec3 Mat4::operator*(const Vec3& vec) const
{
	return Vec3(
		elements[0 + 0 * 4] * vec.x + elements[1 + 0 * 4] * vec.y + elements[2 + 0 * 4] * vec.z + elements[3 + 0 * 4],
		elements[0 + 1 * 4] * vec.x + elements[1 + 1 * 4] * vec.y + elements[2 + 1 * 4] * vec.z + elements[3 + 1 * 4],
		elements[0 + 2 * 4] * vec.x + elements[1 + 2 * 4] * vec.y + elements[2 + 2 * 4] * vec.z + elements[3 + 2 * 4]
	);
}

Mat4 Mat4::operator+=(const Mat4& mat)
{
	*this = *this + mat;
	return *this;
}

Mat4 Mat4::operator-=(const Mat4& mat)
{
	*this = *this - mat;
	return *this;
}

Mat4 Mat4::operator*=(const Mat4& mat)
{
	*this = *this * mat;
	return *this;
}

bool Mat4::operator==(const Mat4& mat) const
{
	for (unsigned int i = 0; i < 4 * 4; i++)
	{
		if (elements[i] != mat.elements[i])
		{
			return false;
		}
	}
	return true;
}

bool Mat4::operator!=(const Mat4& mat) const
{
	return !(*this == mat);
}

const float* Mat4::getElements() const
{
	return elements;
}

Mat4 Mat4::transpose() const
{
	Mat4 result;
	for (unsigned int row = 0; row < 4; row++)
	{
		for (unsigned int column = 0; column < 4; column++)
		{
			result.elements[row + column * 4] = elements[column + row * 4];
		}
	}
	return result;
}

Mat4 Mat4::identity()
{
	return Mat4(1.0f);
}

Mat4 Mat4::lookAt(const Vec3& position, const Vec3& target, const Vec3& worldUp)
{
	// ┌                 ┐
	// │  rx  ry  rz -tx │
	// │  ux  uy  uz -ty │
	// │ -fx -fy -fz  tz │
	// │   0   0   0   1 │
	// └                 ┘
	Vec3 forward = (target - position).normalize();
	Vec3 right = forward.cross(worldUp).normalize();
	Vec3 up = right.cross(forward).normalize();

	Mat4 result(1.0f);
	result(0, 0) = right.x;
	result(0, 1) = right.y;
	result(0, 2) = right.z;
	result(0, 3) = -right.dot(position);

	result(1, 0) = up.x;
	result(1, 1) = up.y;
	result(1, 2) = up.z;
	result(1, 3) = -up.dot(position);

	result(2, 0) = -forward.x;
	result(2, 1) = -forward.y;
	result(2, 2) = -forward.z;
	result(2, 3) = forward.dot(position);

	result(3, 0) = 0.0f;
	result(3, 1) = 0.0f;
	result(3, 2) = 0.0f;
	result(3, 3) = 1.0f;

	return result;
}

Mat4 Mat4::orthographic(const float left, const float right, const float bottom, const float top, const float near, const float far)
{
	Mat4 result(1.0f);
	result(0, 0) = 2.0f / (right - left);
	result(1, 1) = 2.0f / (top - bottom);
	result(2, 2) = 2.0f / (near - far);
	result(3, 0) = (left + right) / (left - right);
	result(3, 1) = (bottom + top) / (bottom - top);
	result(3, 2) = (far + near) / (far - near);
	result(3, 3) = 1.0f;
	return result;
}

Mat4 Mat4::perspective(const float fov, const float aspectRatio, const float near, const float far)
{
	// ┌             ┐
	// │  a  0  0  0 │
	// │  0  q  0  0 │
	// │  0  0  b  c │
	// │  0  0 -1  0 │
	// └             ┘
	Mat4 result(1.0f);
	float q = 1.0f / std::tan(0.5f * fov);
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

Mat4 Mat4::infinitePerspective(const float fov, const float aspectRatio, const float near)
{
	// ┌             ┐
	// │  a  0  0  0 │
	// │  0  q  0  0 │
	// │  0  0  1  c │
	// │  0  0 -1  0 │
	// └             ┘
	Mat4 result(1.0f);
	float q = 1.0f / std::tan(0.5f * fov);
	float a = q / aspectRatio;
	float c = near;
	result(0, 0) = a;
	result(1, 1) = q;
	result(2, 2) = 1.0f;
	result(2, 3) = c;
	result(3, 2) = -1.0f;
	result(3, 3) = 0.0f;
	return result;
}

Mat4 Mat4::translation(const Vec3& translation)
{
	// ┌             ┐
	// │  1  0  0  x │
	// │  0  1  0  y │
	// │  0  0  1  z │
	// │  0  0  0  1 │
	// └             ┘
	Mat4 result(1.0f);
	result(0, 3) = translation.x;
	result(1, 3) = translation.y;
	result(2, 3) = translation.z;
	return result;
}

// https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
Mat4 Mat4::rotation(const float rad, const Vec3& axis)
{
	Mat4 result(1.0f);
	float c = std::cos(rad);
	float ic = 1.0f - c;
	float s = std::sin(rad);
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	result(0, 0) = c + x * x * ic;
	result(1, 0) = y * x * ic + z * s;
	result(2, 0) = z * x * ic - y * s;

	result(0, 1) = x * y * ic - z * s;
	result(1, 1) = c + y * y * ic;
	result(2, 1) = z * y * ic + x * s;

	result(0, 2) = x * z * ic + y * s;
	result(1, 2) = y * z * ic - x * s;
	result(2, 2) = c + z * z * ic;

	return result;
}

Mat4 Mat4::scale(const Vec3& scale)
{
	// ┌             ┐
	// │  x  0  0  0 │
	// │  0  y  0  0 │
	// │  0  0  z  0 │
	// │  0  0  0  1 │
	// └             ┘
	Mat4 result(1.0f);
	result(0, 0) = scale.x;
	result(1, 1) = scale.y;
	result(2, 2) = scale.z;
	return result;
}

std::ostream& operator<<(std::ostream& os, const Mat4& mat)
{
	unsigned int largestWidth = 0;

	for (unsigned int i = 0; i < 4 * 4; i++)
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << mat.elements[i];
		if (ss.str().length() > largestWidth)
		{
			largestWidth = ss.str().length();
		}
	}

	os << "┌ ";
	for (unsigned int i = 0; i < 4; i++)
		os << std::string(largestWidth + 1, ' ');
	os << "┐" << std::endl;

	for (unsigned int row = 0; row < 4; row++)
	{
		os << "│ ";
		for (unsigned int column = 0; column < 4; column++)
		{
			std::stringstream ss;
			ss << std::fixed << std::setprecision(2) << mat.elements[column + row * 4];
			std::string element = ss.str();
			os << std::string(largestWidth - element.length(), ' ') << element;
			if (column != 3)
				os << " ";
		}
		os << " │" << std::endl;
	}

	os << "└ ";
	for (unsigned int i = 0; i < 4; i++)
		os << std::string(largestWidth + 1, ' ');
	os << "┘";

	return os;
}