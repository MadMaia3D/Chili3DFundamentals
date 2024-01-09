#pragma once
#pragma once
#include "Vector3.h"
#include <algorithm>

class ZDepthEffect {
public:
	class Vertex {
	public:
		Vertex(const Vec3& position)
			:
			pos(position){
		}
		Vertex(const Vec3& position, const Vertex& v)
			:
			pos(position){
		}
		Vertex& operator+=(const Vertex& rhs) {
			pos += rhs.pos;
			return *this;
		}
		Vertex operator+(const Vertex& rhs) const {
			return Vertex(*this) += rhs;
		}
		Vertex& operator-=(const Vertex& rhs) {
			pos -= rhs.pos;
			return *this;
		}
		Vertex operator-(const Vertex& rhs) const {
			return Vertex(*this) -= rhs;
		}
		Vertex& operator*=(float rhs) {
			pos *= rhs;
			return *this;
		}
		Vertex operator*(float rhs) const {
			return Vertex(*this) *= rhs;
		}
		Vertex& operator/=(float rhs) {
			pos /= rhs;
			return *this;
		}
		Vertex operator/(float rhs) const {
			return Vertex(*this) /= rhs;
		}
		Vec3 pos{};
	};
public:
	Color operator()(const Vertex& scanPosInfo) {
		const unsigned char z = 255u - std::clamp(unsigned int(scanPosInfo.pos.z * 50), 0u, 255u);
		return Color(z, z, z);
	}
};

