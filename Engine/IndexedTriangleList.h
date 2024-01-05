#pragma once
#include <vector>
#include "Vector3.h"

struct IndexedTriangleList {
	IndexedTriangleList(std::vector<Vec3> vertices, std::vector<int> indices)
		:
		vertices(vertices),
		indices(indices)
	{
		assert(vertices.size() > 2);
		assert(indices.size() % 3 == 0);
		cullFlags.resize(indices.size()/3, false);
	}
	std::vector<Vec3> vertices;
	std::vector<int> indices;
	std::vector<bool> cullFlags;
};