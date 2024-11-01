#pragma once

#include <type_traits>
#include <stdexcept>
#include <immintrin.h>
#include <cmath>
#include "common.h"

namespace chaos
{
	using real = r32;

	namespace math
	{
		template<typename T>
		class Number
		{

			static_assert(std::is_arithmetic<T>::value, "Type supplied to number was not an arithmetic type.");

		public:
			Number(T value);
			~Number();

			T getValue();

			Number<T> clamped(real min, real max);

		private:
			T value;
		};

		template<typename T>
		Number<T>::Number(T value) : value(value) {}

		template<typename T>
		Number<T>::~Number()
		{
		}

		template<typename T>
		T Number<T>::getValue()
		{
			return this->value;
		}

		template<typename T>
		Number<T> Number<T>::clamped(real min, real max)
		{
			T value = this->value;

			if (value < min)
				value = static_cast<T>(min);

			if (value > max)
				value = static_cast<T>(max);

			return Number<T>(value);
		}

		class Float3
		{
		public:

			union
			{
				struct
				{
					r32 x, y, z;
				};

				__m128 vec;
			};

			Float3(r32 x = 0, r32 y = 0, r32 z = 0) : vec({ x, y, z, 0.0f }) {};

			// Math operators
			Float3 operator +(const Float3& other) const
			{
				Float3 result;
				result.vec = _mm_add_ps(this->vec, other.vec);
				return result;
			}

			Float3 operator -(const Float3& other) const
			{
				Float3 result;
				result.vec = _mm_sub_ps(this->vec, other.vec);
				return result;
			}

			Float3 operator *(const r32 scalar) const
			{
				Float3 result;
				result.vec = _mm_mul_ps(this->vec, _mm_set1_ps(scalar));
				return result;
			}

			Float3 operator /(const r32 scalar) const
			{
				Float3 result;
				result.vec = _mm_div_ps(this->vec, _mm_set1_ps(scalar));
				return result;
			}

			// Augment operators
			Float3& operator +=(const Float3& other)
			{
				this->vec = _mm_add_ps(this->vec, other.vec);
				return *this;
			}

			Float3& operator -=(const Float3& other)
			{
				this->vec = _mm_sub_ps(this->vec, other.vec);
				return *this;
			}

			Float3& operator *=(const r32 scalar)
			{
				this->vec = _mm_mul_ps(this->vec, _mm_set1_ps(scalar));
				return *this;
			}

			Float3 operator /=(const r32 scalar)
			{
				this->vec = _mm_div_ps(this->vec, _mm_set1_ps(scalar));
				return *this;
			}

		};

		class Vector3 : public Float3
		{
		public:
			Vector3(r32 x = 0, r32 y = 0, r32 z = 0) : Float3(x, y, z) {}

			void operator =(const Float3& other)
			{
				x = other.x;
				y = other.y;
				z = other.z;
			}

			Vector3(const Float3& copy)
			{
				x = copy.x;
				y = copy.y;
				z = copy.z;
			}

			u32 Dot(const Vector3& other)
			{
				return (x * other.x) + (y * other.y) + (z * other.z);
			}

			Vector3 Normalize() const
			{
				r32 mag = Magnitude();

				return Vector3(x / mag, y / mag, z / mag);
			}

			r32 Magnitude() const
			{
				return sqrt(x * x + y * y + z * z);
			}
		};


		class Matrix4x4
		{
		private:
			r32 matrix[4][4] = {};
			Matrix4x4() = default;
			Matrix4x4(
				r32 r1c1, r32 r1c2, r32 r1c3, r32 r1c4,
				r32 r2c1, r32 r2c2, r32 r2c3, r32 r2c4,
				r32 r3c1, r32 r3c2, r32 r3c3, r32 r3c4,
				r32 r4c1, r32 r4c2, r32 r4c3, r32 r4c4)
			{
				matrix[0][0] = r1c1;
				matrix[0][1] = r1c2;
				matrix[0][2] = r1c3;
				matrix[0][3] = r1c4;

				matrix[1][0] = r2c1;
				matrix[1][1] = r2c2;
				matrix[1][2] = r2c3;
				matrix[1][3] = r2c4;

				matrix[2][0] = r3c1;
				matrix[2][1] = r3c2;
				matrix[2][2] = r3c3;
				matrix[2][3] = r3c4;

				matrix[3][0] = r4c1;
				matrix[3][1] = r4c2;
				matrix[3][2] = r4c3;
				matrix[3][3] = r4c4;
			}

		public:
			static Matrix4x4 Identity()
			{
				return Matrix4x4(
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1);
			}

			static Matrix4x4 Translation(r32 x, r32 y, r32 z)
			{
				return Matrix4x4(
					1, 0, 0, x,
					0, 1, 0, y,
					0, 0, 1, z,
					0, 0, 0, 1);
			}

			static Matrix4x4 Scale(r32 x, r32 y, r32 z)
			{
				return Matrix4x4(
					x, 0, 0, 0,
					0, y, 0, 0,
					0, 0, z, 0,
					0, 0, 0, 1
				);
			}

			static Matrix4x4 ZRotation(r32 angle)
			{
				r32 cosAngle = cos(angle);
				r32 sinAngle = sin(angle);

				return Matrix4x4(
					cosAngle, -sinAngle, 0, 0,
					sinAngle, cosAngle, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1
				);
			}

			static Matrix4x4 YRotation(r32 angle)
			{
				r32 cosAngle = cos(angle);
				r32 sinAngle = sin(angle);

				return Matrix4x4(
					cosAngle, 0, sinAngle, 0,	// First row
					0, 1, 0, 0,					// Second row (no change in Y axis)
					-sinAngle, 0, cosAngle, 0, // Third row
					0, 0, 0, 1               // Fourth row (homogeneous)
				);
			}

			static Matrix4x4 XRotation(r32 angle)
			{
				r32 cosAngle = cos(angle);
				r32 sinAngle = sin(angle);

				return Matrix4x4(
					1, 0, 0, 0,              // First row (no change in X axis)
					0, cosAngle, -sinAngle, 0, // Second row
					0, sinAngle, cosAngle, 0,  // Third row
					0, 0, 0, 1               // Fourth row (homogeneous)
				);
			}

			Matrix4x4 Transpose() const
			{
				return Matrix4x4(
					matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0],
					matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1],
					matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2],
					matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]
				);
			}

			Vector3 operator*(const Vector3& vec) const
			{
				return Vector3(
					matrix[0][0] * vec.x + matrix[0][1] * vec.y + matrix[0][2] * vec.z + matrix[0][3],
					matrix[1][0] * vec.x + matrix[1][1] * vec.y + matrix[1][2] * vec.z + matrix[1][3],
					matrix[2][0] * vec.x + matrix[2][1] * vec.y + matrix[2][2] * vec.z + matrix[2][3]
				);
			}

			Matrix4x4 operator*(const Matrix4x4& other) const
			{
				Matrix4x4 result;
				for (int row = 0; row < 4; ++row)
				{
					for (int col = 0; col < 4; ++col)
					{
						result.matrix[row][col] =
							matrix[row][0] * other.matrix[0][col] +
							matrix[row][1] * other.matrix[1][col] +
							matrix[row][2] * other.matrix[2][col] +
							matrix[row][3] * other.matrix[3][col];
					}
				}
				return result;
			}

		};
	}
}
