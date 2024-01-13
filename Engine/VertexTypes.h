#pragma once
#include "Vector3.h"

class TexturedVertex {
public:
	TexturedVertex() = default;
	TexturedVertex(const Vec3& position, const Vec2& textureCoordinate)
		:
		pos(position),
		tc(textureCoordinate) {
	}
	TexturedVertex(const Vec3& position, const TexturedVertex& v)
		:
		pos(position),
		tc(v.tc) {
	}
	TexturedVertex& operator+=(const TexturedVertex& rhs) {
		pos += rhs.pos;
		tc += rhs.tc;
		return *this;
	}
	TexturedVertex operator+(const TexturedVertex& rhs) const {
		return TexturedVertex(*this) += rhs;
	}
	TexturedVertex& operator-=(const TexturedVertex& rhs) {
		pos -= rhs.pos;
		tc -= rhs.tc;
		return *this;
	}
	TexturedVertex operator-(const TexturedVertex& rhs) const {
		return TexturedVertex(*this) -= rhs;
	}
	TexturedVertex& operator*=(float rhs) {
		pos *= rhs;
		tc *= rhs;
		return *this;
	}
	TexturedVertex operator*(float rhs) const {
		return TexturedVertex(*this) *= rhs;
	}
	TexturedVertex& operator/=(float rhs) {
		pos /= rhs;
		tc /= rhs;
		return *this;
	}
	TexturedVertex operator/(float rhs) const {
		return TexturedVertex(*this) /= rhs;
	}
	Vec3 pos{};
	Vec2 tc{};
};