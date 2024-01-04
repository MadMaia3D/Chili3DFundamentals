#pragma once
#include "Graphics.h"
#include "Vector3.h"

class PreClip3DSpace {
public:
	PreClip3DSpace()
		:
		xFactor(Graphics::ScreenWidth / 2),
		yFactor(Graphics::ScreenHeight / 2)
	{}
	Vec3& Transform(Vec3& v) const {
		const float zInv = 1.0f / v.z;
		v.x = (v.x * zInv + 1.0f) * xFactor;
		v.y = (-v.y * zInv + 1.0f) * yFactor;
		return v;
	}
	Vec3 GetTransformed(const Vec3& v) const {
		return Transform(Vec3(v));
	}
private:
	float xFactor;
	float yFactor;
};