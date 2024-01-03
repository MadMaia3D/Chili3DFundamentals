#pragma once
#include "Graphics.h"
#include "Vector3.h"

class PreClip3DSpace {
public:
	PreClip3DSpace()
		:
		factorX(Graphics::ScreenWidth / 2),
		factorY(Graphics::ScreenHeight / 2)
	{}
	Vec3& Transform(Vec3& v) const {
		v.x = (v.x + 1.0f) * factorX;
		v.y = (-v.y + 1.0f) * factorY;
		return v;
	}
	Vec3 GetTransformed(const Vec3& v) const {
		return Transform(Vec3(v));
	}
private:
	float factorX;
	float factorY;
};