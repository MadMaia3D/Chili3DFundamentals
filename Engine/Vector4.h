#pragma once
#include "Vector3.h"

template <typename T>
class Vector4 : public Vector3<T>
{
public:
	Vector4() = default;
	Vector4(T x, T y, T z, T w = (T)1.0f)
		:
		Vector3(x, y, z),
		w(w) {
	}
	Vector4(const Vector3& v3, T w = (T)1.0f)
		:
		Vector3(v3),
		w(w) {
	}
	template <typename T2>
	explicit operator Vector4<T2>() const {
		return { (T2)x,(T2)y,(T2)z,(T2)w };
	}
	Vector4	operator-() const {
		return Vector4(-x, -y, -z, -w);
	}
	Vector4& operator+=(const Vector4 &rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}
	Vector4& operator-=(const Vector4 &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}
	Vector4	operator+(const Vector4 &rhs) const {
		return Vector4(*this) += rhs;
	}
	Vector4	operator-(const Vector4 &rhs) const {
		return Vector4(*this) -= rhs;
	}
	Vector4& operator*=(const T &rhs) {
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;
		return *this;
	}
	Vector4	operator*(const T &rhs) const {
		return Vector4(*this) *= rhs;
	}
	Vector4& operator/=(const T &rhs) {
		x /= rhs;
		y /= rhs;
		z /= rhs;
		w /= rhs;
		return *this;
	}
	Vector4	operator/(const T &rhs) const {
		return Vector4(*this) /= rhs;
	}
	bool operator==(const Vector4 &rhs) const {
		return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
	}
	bool operator!=(const Vector4 &rhs) const {
		return !(*this == rhs);
	}

	Vector4& Hadamard(const Vector4 &other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;
		return *this;
	}
	Vector4 GetHadamard(const Vector4 &other) const {
		Vector4 output = *this;
		return output.Hadamard(other);
	}
	Vector4& Saturate() {
		x = std::clamp(x, T(0), T(1));
		y = std::clamp(y, T(0), T(1));
		z = std::clamp(z, T(0), T(1));
		w = std::clamp(w, T(0), T(1));
		return *this;
	}
	Vector4 GetSaturated() const {
		Vector4 output = *this;
		return output.Saturate();
	}
public:
	T w;
};

typedef Vector4<float> Vec4;
typedef Vector4<double> Ved4;
typedef Vector4<int> Vei4;