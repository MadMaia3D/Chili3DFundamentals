#pragma once
#include "IndexedLineList.h"
#include "IndexedTriangleList.h"
#include "TexVertex.h"

class Cube {
public:
	Cube(float size) {
		float half = size / 2.0f;
		vertices.reserve(8);
		vertices.emplace_back(-half, half, -half);		// index 0
		textureCoordinates.emplace_back(0.0f, 0.0f);
		vertices.emplace_back(half, half, -half);		// index 1
		textureCoordinates.emplace_back(1.0f, 0.0f);
		vertices.emplace_back(half, -half, -half);		// index 2
		textureCoordinates.emplace_back(1.0f, 1.0f);
		vertices.emplace_back(-half, -half, -half);		// index 3
		textureCoordinates.emplace_back(0.0f, 1.0f);
		vertices.emplace_back(-half, half, half);		// index 4
		textureCoordinates.emplace_back(1.0f, 0.0f);
		vertices.emplace_back(half, half, half);		// index 5
		textureCoordinates.emplace_back(0.0f, 0.0f);
		vertices.emplace_back(half, -half, half);		// index 6
		textureCoordinates.emplace_back(0.0f, 1.0f);
		vertices.emplace_back(-half, -half, half);		// index 7
		textureCoordinates.emplace_back(1.0f, 1.0f);
	}
	IndexedLineList GetLines() const {
		return {
			vertices, {
			0,1, 1,2, 2,3, 3,0,
			4,5, 5,6, 6,7 ,7,4,
			0,4, 1,5, 2,6, 3,7} };
	}

	IndexedTriangleList<Vec3> GetTriangles() const {
		return {
			vertices, {
			0,1,2,	2,3,0,
			1,5,2,	5,6,2,
			4,6,5,	4,7,6,
			0,7,4,	0,3,7,
			0,4,5,	0,5,1,
			2,7,3,	2,6,7
		} };
	}

	IndexedTriangleList<TexVertex> GetTrianglesTex() const {
		std::vector<TexVertex> tverts;
		tverts.reserve(vertices.size());

		for (size_t i = 0; i < vertices.size(); i++) {
			tverts.emplace_back(vertices[i], textureCoordinates[i]);
		}
		return {
			std::move(tverts), {
			0,1,2,	2,3,0,
			1,5,2,	5,6,2,
			4,6,5,	4,7,6,
			0,7,4,	0,3,7,
			0,4,5,	0,5,1,
			2,7,3,	2,6,7
		} };
	}

private:
	std::vector<Vec3> vertices;
	std::vector<Vec2> textureCoordinates;
};