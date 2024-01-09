#pragma once
#include "Vector3.h"
#include "Colors.h"

class VertexColorEffect{
public:
	class Vertex {
	public:
		Vertex(const Vec3& position)
			:
			pos(position),
			color(128.0f, 128.0f, 128.0f) {
		}
		Vertex(const Vec3& position, const Vec3& color)
			:
			pos(position),
			color(color) {
		}
		Vertex(const Vec3& position, const Vertex& v)
			:
			pos(position),
			color(v.color) {
		}
		Vertex& operator+=(const Vertex& rhs) {
			pos += rhs.pos;
			color += rhs.color;
			return *this;
		}
		Vertex operator+(const Vertex& rhs) const {
			return Vertex(*this) += rhs;
		}
		Vertex& operator-=(const Vertex& rhs) {
			pos -= rhs.pos;
			color -= rhs.color;
			return *this;
		}
		Vertex operator-(const Vertex& rhs) const {
			return Vertex(*this) -= rhs;
		}
		Vertex& operator*=(float rhs) {
			pos *= rhs;
			color *= rhs;
			return *this;
		}
		Vertex operator*(float rhs) const {
			return Vertex(*this) *= rhs;
		}
		Vertex& operator/=(float rhs) {
			pos /= rhs;
			color /= rhs;
			return *this;
		}
		Vertex operator/(float rhs) const {
			return Vertex(*this) /= rhs;
		}
		Vec3 pos{};
		Vec3 color{};
	};
public:
	Color operator()(const Vertex& scanPosInfo) {
		return Color(scanPosInfo.color);
	}
};

