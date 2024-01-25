#pragma once
#include "Vector3.h"

template <typename T, size_t S>
class Matrix {
public:
	Matrix& operator=(const Matrix& rhs) {
		memcpy(elements, rhs.elements, sizeof(elements));
		return *this;
	}
	Matrix& operator*=(T rhs) {
		for (auto& row : elements) {
			for (T& e : row) {
				e *= rhs;
			}
		}
		return *this;
	}

	Matrix operator*(T rhs) const {
		Matrix result = *this;
		return result *= rhs;
	}

	Matrix operator*(const Matrix& rhs) const {
		Matrix result;
		for (size_t j = 0; j < S; j++) {
			for (size_t k = 0; k < S; k++) {
				T sum = (T)0.0;
				for (size_t i = 0; i < S; i++) {
					sum += elements[j][i] * rhs.elements[i][k];
				}
				result.elements[j][k] = sum;
			}
		}
		return result;
	}

	static Matrix Identity() {
		if constexpr (S == 4) {
			return	{ (T)1.0,(T)0.0,(T)0.0,(T)0.0,
						(T)0.0,(T)1.0,(T)0.0,(T)0.0,
						(T)0.0,(T)0.0,(T)1.0,(T)0.0,
						(T)0.0,(T)0.0,(T)0.0,(T)1.0, };
			
		} else if constexpr (S == 3) {
			return	{ (T)1.0,(T)0.0,(T)0.0,
						(T)0.0,(T)1.0,(T)0.0,
						(T)0.0,(T)0.0,(T)1.0 };
		} else {
			static_assert("Bad Indentity Matrix Size");
		}
	}

	static Matrix Scaling(T factor) {
		if constexpr (S == 4) {
			return { factor,(T)0.0,(T)0.0,
					 (T)0.0,factor,(T)0.0,
					 (T)0.0,(T)0.0, factor };
		} else if constexpr (S == 3) {
			return {	factor,(T)0.0,(T)0.0,(T)0.0,
						(T)0.0,factor,(T)0.0,(T)0.0,
						(T)0.0,(T)0.0, factor,(T)0.0,
						(T)0.0, (T)0.0, (T)0.0, (T)1.0};
		} else {
			static_assert("Bad Scale Matrix Size");
		}
	}

	static Matrix RotationZ(T theta) {
		const T sinTheta = sin(theta);
		const T cosTheta = cos(theta);
		if constexpr (S == 4) {
			return {cosTheta, sinTheta, T(0), T(0),
					-sinTheta, cosTheta, T(0), T(0),
					T(0)	, T(0)	, T(1), T(0), 
					T(0)	, T(0)	, T(0), T(1), };
		} else if constexpr (S == 3) {
			return { cosTheta, sinTheta, T(0),
					-sinTheta, cosTheta, T(0),
					T(0), T(0), T(1) };
		} else {
			static_assert("Bad RotationZ Matrix Size");
		}
	}

	static Matrix RotationY(T theta) {
		const T sinTheta = sin(theta);
		const T cosTheta = cos(theta);
		if constexpr (S == 4) {
			return {cosTheta, T(0), -sinTheta,	T(0),
					T(0)	, T(1),	T(0)	 ,	T(0),
					sinTheta, T(0), cosTheta ,	T(0),
					T(0)	, T(0), T(0)	 ,	T(1) };
		} else if constexpr (S == 3) {
			return { cosTheta, T(0), -sinTheta,
					T(0)	 , T(1),	T(0),
					sinTheta , T(0), cosTheta };
		} else {
			static_assert("Bad RotationY Matrix Size");
		}
	}

	static Matrix RotationX(T theta) {
		const T sinTheta = sin(theta);
		const T cosTheta = cos(theta);
		if constexpr (S == 4) {
			return {	T(1), T(0)		,	T(0)	,T(0),
						T(0), cosTheta	, sinTheta	,T(0),
						T(0), -sinTheta	, cosTheta	,T(0), 
						T(0),	T(0)	,	T(0)	,T(1) };
		} else if constexpr (S == 3) {
			return {	T(1), T(0)		, T(0),
						T(0), cosTheta	, sinTheta,
						T(0), -sinTheta	, cosTheta };
		} else {
			static_assert("Bad RotationX Matrix Size");
		}
	}

	static Matrix Translation(T Tx, T Ty,T Tz) {
		if constexpr (S == 4) {
			return {	T(1), T(0) , T(0) ,T(0),
						T(0), T(1) , T(0) ,T(0),
						T(0), T(0) , T(1) ,T(0),
						Tx,	  Ty ,   Tz ,T(1), };
		} else {
			static_assert("Bad RotationX Matrix Size");
		}
	}
public:
	// [ row ][ col ]
	T elements[S][S];
};

template<typename T>
Vector3<T>& operator*=(Vector3<T>& lhs, const Matrix<T,3>& rhs) {
	return lhs = lhs * rhs;
}

template<typename T>
Vector3<T> operator*(const Vector3<T>& lhs, const Matrix<T,3>& rhs) {
	return {
		lhs.x * rhs.elements[0][0] + lhs.y * rhs.elements[1][0] + lhs.z * rhs.elements[2][0],
		lhs.x * rhs.elements[0][1] + lhs.y * rhs.elements[1][1] + lhs.z * rhs.elements[2][1],
		lhs.x * rhs.elements[0][2] + lhs.y * rhs.elements[1][2] + lhs.z * rhs.elements[2][2]
	};
}

typedef Matrix<float,3> Mat3;
typedef Matrix<double,3> Mad3;