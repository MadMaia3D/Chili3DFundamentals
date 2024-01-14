#pragma once
#include "Vector3.h"

namespace VertexTypes {
	class ColorVertex {
	public:
		ColorVertex(const Vec3& position)
			:
			pos(position),
			color(128.0f, 128.0f, 128.0f) {
		}
		ColorVertex(const Vec3& position, const Vec3& color)
			:
			pos(position),
			color(color) {
		}
		ColorVertex(const Vec3& position, const ColorVertex& v)
			:
			pos(position),
			color(v.color) {
		}
		ColorVertex& operator+=(const ColorVertex& rhs) {
			pos += rhs.pos;
			color += rhs.color;
			return *this;
		}
		ColorVertex operator+(const ColorVertex& rhs) const {
			return ColorVertex(*this) += rhs;
		}
		ColorVertex& operator-=(const ColorVertex& rhs) {
			pos -= rhs.pos;
			color -= rhs.color;
			return *this;
		}
		ColorVertex operator-(const ColorVertex& rhs) const {
			return ColorVertex(*this) -= rhs;
		}
		ColorVertex& operator*=(float rhs) {
			pos *= rhs;
			color *= rhs;
			return *this;
		}
		ColorVertex operator*(float rhs) const {
			return ColorVertex(*this) *= rhs;
		}
		ColorVertex& operator/=(float rhs) {
			pos /= rhs;
			color /= rhs;
			return *this;
		}
		ColorVertex operator/(float rhs) const {
			return ColorVertex(*this) /= rhs;
		}
		Vec3 pos{};
		Vec3 color{};
	};
}