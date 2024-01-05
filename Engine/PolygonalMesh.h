#pragma once
#include "IndexedLineList.h"
#include "IndexedTriangleList.h"

class Cube {
public:
	Cube(float size) {
		float half = size / 2.0f;
		vertices.reserve(8);
		vertices.emplace_back(-half, half, -half);
		vertices.emplace_back(half, half, -half);
		vertices.emplace_back(half, -half, -half);
		vertices.emplace_back(-half, -half, -half);
		vertices.emplace_back(-half, half, half);
		vertices.emplace_back(half, half, half);
		vertices.emplace_back(half, -half, half);
		vertices.emplace_back(-half, -half, half);
	}
	IndexedLineList GetLines() const {
		return {
			vertices, {
			0,1, 1,2, 2,3, 3,0,
			4,5, 5,6, 6,7 ,7,4,
			0,4, 1,5, 2,6, 3,7} };
	}
	IndexedTriangleList GetTriangles() const {
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
private:
	std::vector<Vec3> vertices;
};