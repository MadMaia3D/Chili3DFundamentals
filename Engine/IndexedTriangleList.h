#pragma once
#include <vector>
#include "Vector3.h"

struct IndexedTriangleList {
	std::vector<Vec3> vertices;
	std::vector<int> indices;
};