#pragma once
#include "Vector3.h"

namespace VertexTypes {
	class PlainPositionVertex {
	public:
		PlainPositionVertex() = default;
		PlainPositionVertex(const Vec3& position)
			:
			pos(position) {
		}
		PlainPositionVertex(const Vec3& position, const PlainPositionVertex& v)
			:
			pos(position) {
		}
		PlainPositionVertex& operator+=(const PlainPositionVertex& rhs) {
			pos += rhs.pos;
			return *this;
		}
		PlainPositionVertex operator+(const PlainPositionVertex& rhs) const {
			return PlainPositionVertex(*this) += rhs;
		}
		PlainPositionVertex& operator-=(const PlainPositionVertex& rhs) {
			pos -= rhs.pos;
			return *this;
		}
		PlainPositionVertex operator-(const PlainPositionVertex& rhs) const {
			return PlainPositionVertex(*this) -= rhs;
		}
		PlainPositionVertex& operator*=(float rhs) {
			pos *= rhs;
			return *this;
		}
		PlainPositionVertex operator*(float rhs) const {
			return PlainPositionVertex(*this) *= rhs;
		}
		PlainPositionVertex& operator/=(float rhs) {
			pos /= rhs;
			return *this;
		}
		PlainPositionVertex operator/(float rhs) const {
			return PlainPositionVertex(*this) /= rhs;
		}
		Vec3 pos{};
	};
}