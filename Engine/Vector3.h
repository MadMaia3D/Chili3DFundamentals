#pragma once

template <typename T>
class Vector3
{
public:
	Vector3() {}
	Vector3(T x, T y, T z)
		:
		x(x),
		y(y),
		z(z)
	{}
	Vector3(const Vector3& vec)
		:
		Vector3(vec.x, vec.y, vec.z)
	{}
	template <typename T2>
	explicit operator Vector3<T2>() const
	{
		return { (T2)x,(T2)y, (T2)z };
	}
	T LenSquared() const {
		return sq(*this);
	}
	T Lenght() const {
		return sqrt(LenSq());
	}
	Vector3& Normalize() {
		const T length = Lenght();
		x /= length;
		y /= length;
		z /= length;
		return *this;
	}
	Vector3	GetNormalized() const {
		Vector3 norm = *this;
		norm.Normalize();
		return norm;
	}
	Vector3	operator-() const {
		return Vector3(-x, -y, -z);
	}
	Vector3& operator=(const Vector3 &rhs)	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}
	Vector3& operator+=(const Vector3 &rhs)	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	Vector3& operator-=(const Vector3 &rhs)	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	T operator*(const Vector3 &rhs) const	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}
	Vector3	operator+(const Vector3 &rhs) const	{
		return Vector3(*this) += rhs;
	}
	Vector3	operator-(const Vector3 &rhs) const	{
		return Vector3(*this) -= rhs;
	}
	Vector3& operator*=(const T &rhs) {
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}
	Vector3	operator*(const T &rhs) const {
		return Vector3(*this) *= rhs;
	}
	Vector3& operator/=(const T &rhs) {
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}
	Vector3	operator/(const T &rhs) const {
		return Vector3(*this) /= rhs;
	}
	bool operator==(const Vector3 &rhs) const {
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}
	bool operator!=(const Vector3 &rhs) const {
		return !(*this == rhs);
	}
public:
	T x;
	T y;
	T z;
};

typedef Vector3<float> Vec3;
typedef Vector3<double> Ved3;
typedef Vector3<int> Vei3;