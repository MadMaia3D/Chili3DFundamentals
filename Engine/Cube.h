#pragma once
#include "IndexedTriangleList.h"

class Cube {
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
	static IndexedTriangleList<Vertex> GetNormalsCube(float size = 1.0f) {
		IndexedTriangleList<Vertex> vertices = GetSplitCube<Vertex>(size);
		// front normals
		vertices.vertices[0].normal = Vec3{ 0.0f, 0.0f, -1.0f };
		vertices.vertices[1].normal = Vec3{ 0.0f, 0.0f, -1.0f };
		vertices.vertices[2].normal = Vec3{ 0.0f, 0.0f, -1.0f };
		vertices.vertices[3].normal = Vec3{ 0.0f, 0.0f, -1.0f };
		// right normals
		vertices.vertices[4].normal = Vec3{ 1.0f, 0.0f, 0.0f };
		vertices.vertices[5].normal = Vec3{ 1.0f, 0.0f, 0.0f };
		vertices.vertices[6].normal = Vec3{ 1.0f, 0.0f, 0.0f };
		vertices.vertices[7].normal = Vec3{ 1.0f, 0.0f, 0.0f };
		// back normals
		vertices.vertices[8].normal = Vec3{ 0.0f, 0.0f, 1.0f };
		vertices.vertices[9].normal = Vec3{ 0.0f, 0.0f, 1.0f };
		vertices.vertices[10].normal = Vec3{ 0.0f, 0.0f, 1.0f };
		vertices.vertices[11].normal = Vec3{ 0.0f, 0.0f, 1.0f };
		// left normals
		vertices.vertices[12].normal = Vec3{ -1.0f, 0.0f, 0.0f };
		vertices.vertices[13].normal = Vec3{ -1.0f, 0.0f, 0.0f };
		vertices.vertices[14].normal = Vec3{ -1.0f, 0.0f, 0.0f };
		vertices.vertices[15].normal = Vec3{ -1.0f, 0.0f, 0.0f };
		// top normals
		vertices.vertices[16].normal = Vec3{ 0.0f, 1.0f, 0.0f };
		vertices.vertices[17].normal = Vec3{ 0.0f, 1.0f, 0.0f };
		vertices.vertices[18].normal = Vec3{ 0.0f, 1.0f, 0.0f };
		vertices.vertices[19].normal = Vec3{ 0.0f, 1.0f, 0.0f };
		// bottom normals
		vertices.vertices[20].normal = Vec3{ 0.0f, -1.0f, 0.0f };
		vertices.vertices[21].normal = Vec3{ 0.0f, -1.0f, 0.0f };
		vertices.vertices[22].normal = Vec3{ 0.0f, -1.0f, 0.0f };
		vertices.vertices[23].normal = Vec3{ 0.0f, -1.0f, 0.0f };
		return vertices;
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


};