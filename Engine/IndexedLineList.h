#pragma once
#include <vector>
#include "Vector3.h"

struct IndexedLineList {
	std::vector<Vec3> vertices;
	std::vector<int> indices;
};