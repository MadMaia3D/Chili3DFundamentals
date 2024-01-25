#pragma once
#include "Vector2.h"
#include "Vector3.h"

class TexVertex {
public:
	TexVertex(const Vec3& position, const Vec2& textureCoordinate)
		:
		pos(position),
		tc(textureCoordinate)
	{}
	Vec3 pos;
	Vec2 tc;
};