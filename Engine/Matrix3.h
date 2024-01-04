#pragma once
#include "Vector3.h"

template <typename T>
class Matrix3
{
public:
	Matrix3& operator=(const Matrix3& rhs)
	{
		memcpy(elements, rhs.elements, sizeof(elements));
		return *this;
	}
	Matrix3& operator*=(T rhs)
	{
		for (auto& row : elements)
		{
			for (T& e : row)
			{
				e *= rhs;
			}
		}
		return *this;
	}
	Matrix3 operator*(T rhs) const
	{
		Matrix3 result = *this;
		return result *= rhs;
	}
	Matrix3 operator*(const Matrix3& rhs) const
	{
		Matrix3 result;
		for (size_t j = 0; j < 3; j++)
		{
			for (size_t k = 0; k < 3; k++)
			{
				T sum = (T)0.0;
				for (size_t i = 0; i < 3; i++)
				{
					sum += elements[j][i] * rhs.elements[i][k];
				}
				result.elements[j][k] = sum;
			}
		}
		return result;
	}
	static Matrix3 Identity()
	{
		Matrix3 i = { (T)1.0,(T)0.0,(T)0.0,
			          (T)0.0,(T)1.0,(T)0.0,
		              (T)0.0,(T)0.0,(T)1.0};
		return i;
	}
	static Matrix3 Scaling(T factor)
	{
		Matrix3 s = { factor,(T)0.0,(T)0.0,
			         (T)0.0,factor,(T)0.0,
					 (T)0.0,(T)0.0, factor };
		return s;
	}
	static Matrix3 RotationZ(T theta) {
		const T sinTheta = sin(theta);
		const T cosTheta = cos(theta);
		return {
			cosTheta, sinTheta, T(0),
			-sinTheta, cosTheta, T(0),
			T(0), T(0), T(1)
		};
	}
	static Matrix3 RotationY(T theta) {
		const T sinTheta = sin(theta);
		const T cosTheta = cos(theta);
		return {
			cosTheta, T(0), -sinTheta,
			T(0),	  T(1),	T(0),
			sinTheta, T(0), cosTheta
		};
	}
	static Matrix3 RotationX(T theta) {
		const T sinTheta = sin(theta);
		const T cosTheta = cos(theta);
		return {
			T(1), T(0),		T(0),
			T(0), cosTheta, sinTheta,
			T(0), -sinTheta, cosTheta
		};
	}
public:
	// [ row ][ col ]
	T elements[3][3];
};

template<typename T>
Vector3<T>& operator*=(Vector3<T>& lhs, const Matrix3<T>& rhs)
{
	return lhs = lhs * rhs;
}

template<typename T>
Vector3<T> operator*(const Vector3<T>& lhs, const Matrix3<T>& rhs)
{
	return {
		lhs.x * rhs.elements[0][0] + lhs.y * rhs.elements[1][0] + lhs.z * rhs.elements[2][0],
		lhs.x * rhs.elements[0][1] + lhs.y * rhs.elements[1][1] + lhs.z * rhs.elements[2][1],
		lhs.x * rhs.elements[0][2] + lhs.y * rhs.elements[1][2] + lhs.z * rhs.elements[2][2]
	};
}

typedef Matrix3<float> Mat3;
typedef Matrix3<double> Mad3;