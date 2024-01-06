#pragma once
#include "Vec2.h"
#include "Vector3.h"

class TexVertex {
public:
	TexVertex(const Vec3& position, const Vec2& textureCoordinate)
		:
		pos(position),
		tc(textureCoordinate)
	{}
	TexVertex InterpolateTo(const TexVertex& destination, float alpha) const {
		return { pos.InterpolateTo(destination.pos, alpha), tc.InterpolateTo(destination.tc, alpha) };
	}
	Vec3 pos;
	Vec2 tc;
};