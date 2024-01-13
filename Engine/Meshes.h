#pragma once
#include "IndexedTriangleList.h"
#include <array>

class Meshes {
public:
	template<typename Vertex>
	static IndexedTriangleList<Vertex> GetTexturedCube(float size = 1.0f) {
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
	static IndexedTriangleList<Vertex> GetColorVertexCube(float size = 1.0f) {
		float half = size / 2;
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

	template<typename Vertex>
	static IndexedTriangleList<Vertex> GetSkinnedCube(float size = 1.0f) {
		float half = size / 2.0f;
		std::vector<Vertex> verts;
#include "./Models/skinned_cube_vertices.txt"
		verts.shrink_to_fit();
		return {
			std::move(verts), {
#include "./Models/skinned_cube_indices.txt"
		} };
	}


	template<typename Vertex>
	static IndexedTriangleList<Vertex> GetSuzanne() {
		std::vector<Vertex> verts;
#include "./Models/suzanne_vertices.txt"
		verts.shrink_to_fit();
		return {
			std::move(verts), {
#include "./Models/suzanne_indices.txt"
		} };
	}

	template<typename Vertex>
	static IndexedTriangleList<Vertex> GetPlane(float size = 1.0f, int divisions = 8) {
		// divisions is the number of edges at each side of the plane
		// nVertexSide is the number of edges at each side of the plane
		const int nVerticesSide = divisions + 1;
		const float halfSide = size / 2.0f;
		const float segmentSide = size / float(divisions);

		std::vector<Vertex> vertices(sq(nVerticesSide));

		// generate positions
		const Vec3 startPosition = { -halfSide, -halfSide, 0.0f };
		for (int y = 0, i = 0; y < nVerticesSide; y++) {
			for (int x = 0; x < nVerticesSide; x++, i++) {
				Vec3 a = { x * segmentSide, y * segmentSide, 0.0f };
				Vec3 result = startPosition + a;
				vertices[i].pos = result;
			}
		}

		std::vector<size_t> indices;
		indices.reserve(sq(divisions) * 6);
		// generate indices
		const auto xy_to_index = [&nVerticesSide](size_t x, size_t y) { return y * nVerticesSide + x; };
		for (int iy = 0; iy < divisions; iy++) {
			for (int ix = 0; ix < divisions; ix++) {
				std::array<size_t, 4> quadIndices {
					xy_to_index(ix, iy) ,xy_to_index(ix + 1, iy) ,xy_to_index(ix + 1, iy + 1) ,xy_to_index(ix, iy + 1) };
				indices.push_back(quadIndices[0]);
				indices.push_back(quadIndices[2]);
				indices.push_back(quadIndices[1]);
				indices.push_back(quadIndices[0]);
				indices.push_back(quadIndices[3]);
				indices.push_back(quadIndices[2]);
			}
		}

		// generate coordinates
		const float tcStep = 1.0f / float(divisions);
		for (int y = 0, i = 0; y < nVerticesSide; y++) {
			for (int x = 0; x < nVerticesSide; x++, i++) {
				vertices[i].tc = { float(x * tcStep), float(y * tcStep) };
			}
		}
		return IndexedTriangleList<Vertex>(vertices, indices);
	}
};