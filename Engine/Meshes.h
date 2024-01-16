#pragma once
#include "IndexedTriangleList.h"
#include <array>

class Meshes {
public:
	template<typename Vertex>
	static IndexedTriangleList<Vertex> GetCube(float size = 1.0f) {
		float half = size / 2.0f;
		std::vector<Vertex> vertices(8);
		vertices[0].pos = { -half, half, -half };
		vertices[1].pos = { half, half, -half };
		vertices[2].pos = { half, -half, -half };
		vertices[3].pos = { -half, -half, -half };
		vertices[4].pos = { -half, half, half };
		vertices[5].pos = { half, half, half };
		vertices[6].pos = { half, -half, half };
		vertices[7].pos = { -half, -half, half };
		return {
			std::move(vertices), {
			0,1,2,	2,3,0,
			1,5,2,	5,6,2,
			4,6,5,	4,7,6,
			0,7,4,	0,3,7,
			0,4,5,	0,5,1,
			2,7,3,	2,6,7
		} };
	}

	template <typename Vertex>
	static IndexedTriangleList<Vertex> GetTexturedCube(float size = 1.0f) {
		IndexedTriangleList<Vertex> vertices = GetCube<Vertex>(size);
		vertices.vertices[0].tc = { 0.0f, 0.0f };
		vertices.vertices[1].tc = { 1.0f, 0.0f };
		vertices.vertices[2].tc = { 1.0f, 1.0f };
		vertices.vertices[3].tc = { 0.0f, 1.0f };
		vertices.vertices[4].tc = { 1.0f, 0.0f };
		vertices.vertices[5].tc = { 0.0f, 0.0f };
		vertices.vertices[6].tc = { 0.0f, 1.0f };
		vertices.vertices[7].tc = { 1.0f, 1.0f };
		return vertices;
	}

	template<typename Vertex>
	static IndexedTriangleList<Vertex> GetSplitCube(float size = 1.0f) {
		float half = size / 2.0f;
		std::vector<Vertex> vertices(24);
		// front
		vertices[0].pos = { -half, half, -half };
		vertices[1].pos = { half, half, -half };
		vertices[2].pos = { half, -half, -half };
		vertices[3].pos = { -half, -half, -half };
		// right
		vertices[4].pos = { half, half, -half };
		vertices[5].pos = { half, half, half };
		vertices[6].pos = { half, -half, half };
		vertices[7].pos = { half, -half, -half };
		// back
		vertices[8].pos = { half, half, half };
		vertices[9].pos = { -half, half, half };
		vertices[10].pos = { -half, -half, half };
		vertices[11].pos = { half, -half, half };
		// left
		vertices[12].pos = { -half, half, half };
		vertices[13].pos = { -half, half, -half };
		vertices[14].pos = { -half, -half, -half };
		vertices[15].pos = { -half, -half, half };
		// top
		vertices[16].pos = { half, half, -half };
		vertices[17].pos = { -half, half, -half };
		vertices[18].pos = { -half, half, half };
		vertices[19].pos = { half, half, half };
		// bottom
		vertices[20].pos = { -half, -half, -half };
		vertices[21].pos = { half, -half, -half };
		vertices[22].pos = { half, -half, half };
		vertices[23].pos = { -half, -half, half };
		return {
			std::move(vertices), {
			0,1,2,		2,3,0,
			4,5,6,		6,7,4,
			8,9,10,		10,11,8,
			12,13,14,	14,15,12,
			16,17,18,	18,19,16,
			20,21,22,	22,23,20
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
		return IndexedTriangleList<Vertex>(vertices, indices);
	}

	template<typename Vertex>
	static IndexedTriangleList<Vertex> GetSkinnedPlane(float size = 1.0f, int divisions = 8) {
		IndexedTriangleList<Vertex> vertices = GetPlane<Vertex>(size, divisions);
		const int nVerticesSide = divisions + 1;

		// generate coordinates
		const float tcStep = 1.0f / float(divisions);
		for (int y = 0, i = 0; y < nVerticesSide; y++) {
			for (int x = 0; x < nVerticesSide; x++, i++) {
				vertices.vertices[i].tc = { float(x * tcStep), 1.0f - float(y * tcStep) };
			}
		}
		return vertices;
	}
};