#pragma once
#include "IndexedTriangleList.h"

class Cube {
public:
	template<typename Vertex>
	static IndexedTriangleList<Vertex> GetTexturedCube(float size) {
		float half = size / 2.0f;
		std::vector<Vertex> verts;
		verts.reserve(8);
		verts.emplace_back(Vec3{ -half, half, -half }, Vec2{ 0.0f, 0.0f });
		verts.emplace_back(Vec3{ half, half, -half }, Vec2{ 1.0f, 0.0f });
		verts.emplace_back(Vec3{ half, -half, -half }, Vec2{ 1.0f, 1.0f });
		verts.emplace_back(Vec3{ -half, -half, -half }, Vec2{ 0.0f, 1.0f });
		verts.emplace_back(Vec3{ -half, half, half }, Vec2{ 1.0f, 0.0f });
		verts.emplace_back(Vec3{ half, half, half }, Vec2{ 0.0f, 0.0f });
		verts.emplace_back(Vec3{ half, -half, half }, Vec2{ 0.0f, 1.0f });
		verts.emplace_back(Vec3{ -half, -half, half }, Vec2{ 1.0f, 1.0f });
		return {
			std::move(verts), {
			0,1,2,	2,3,0,
			1,5,2,	5,6,2,
			4,6,5,	4,7,6,
			0,7,4,	0,3,7,
			0,4,5,	0,5,1,
			2,7,3,	2,6,7
		} };
	}

	template<typename Vertex>
	static IndexedTriangleList<Vertex> GetColorVertexCube(float size) {
		float half = size / 2.0f;
		std::vector<Vertex> verts;
		verts.reserve(8);
		verts.emplace_back(Vec3{ -half, half, -half });
		verts.emplace_back(Vec3{ half, half, -half });
		verts.emplace_back(Vec3{ half, -half, -half });
		verts.emplace_back(Vec3{ -half, -half, -half });
		verts.emplace_back(Vec3{ -half, half, half });
		verts.emplace_back(Vec3{ half, half, half });
		verts.emplace_back(Vec3{ half, -half, half });
		verts.emplace_back(Vec3{ -half, -half, half });
		return {
			std::move(verts), {
			0,1,2,	2,3,0,
			1,5,2,	5,6,2,
			4,6,5,	4,7,6,
			0,7,4,	0,3,7,
			0,4,5,	0,5,1,
			2,7,3,	2,6,7
		} };
	}
};