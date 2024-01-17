#pragma once
#include "Vector3.h"

namespace VertexTypes {
	class NormalsVertex {
	public:
		NormalsVertex() = default;
		NormalsVertex(const Vec3& position, const Vec3& normal)
			:
			pos(position),
			normal(normal) {
		}
		NormalsVertex(const Vec3& position, const NormalsVertex& v)
			:
			pos(position),
			normal(v.normal) {
		}
		NormalsVertex& operator+=(const NormalsVertex& rhs) {
			pos += rhs.pos;
			normal += rhs.normal;
			return *this;
		}
		NormalsVertex operator+(const NormalsVertex& rhs) const {
			return NormalsVertex(*this) += rhs;
		}
		NormalsVertex& operator-=(const NormalsVertex& rhs) {
			pos -= rhs.pos;
			normal -= rhs.normal;
			return *this;
		}
		NormalsVertex operator-(const NormalsVertex& rhs) const {
			return NormalsVertex(*this) -= rhs;
		}
		NormalsVertex& operator*=(float rhs) {
			pos *= rhs;
			normal *= rhs;
			return *this;
		}
		NormalsVertex operator*(float rhs) const {
			return NormalsVertex(*this) *= rhs;
		}
		NormalsVertex& operator/=(float rhs) {
			pos /= rhs;
			normal /= rhs;
			return *this;
		}
		NormalsVertex operator/(float rhs) const {
			return NormalsVertex(*this) /= rhs;
		}
		Vec3 pos{};
		Vec3 normal{};
	};
}