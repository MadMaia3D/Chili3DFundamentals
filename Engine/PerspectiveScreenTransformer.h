#pragma once
#include "Graphics.h"
#include "Vector3.h"

class PerspectiveScreenTransformer {
public:
	PerspectiveScreenTransformer()
		:
		xFactor(Graphics::ScreenWidth / 2),
		yFactor(Graphics::ScreenHeight / 2)
	{}
	template<typename Vertex>
	Vertex& TransformVertex(Vertex& v) const {
		const float zInv = 1.0f / v.pos.z;
		v *= zInv;
		v.pos.x = (v.pos.x + 1.0f) * xFactor;
		v.pos.y = (-v.pos.y + 1.0f) * yFactor;
		v.pos.z = zInv;
		return v;
	}
	template<typename Vertex>
	Vertex GetTransformedVertex(const Vertex& v) const {
		return Transform(Vertex(v));
	}
private:
	float xFactor;
	float yFactor;
};