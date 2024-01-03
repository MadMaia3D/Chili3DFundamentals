#pragma once
#include "IndexedLineList.h"


class Cube {
public:
	Cube(float size){
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
private:
	std::vector<Vec3> vertices;
};