#pragma once
#include <vector>
#include "Vector3.h"

template <class T>
class IndexedTriangleList {
public:
	IndexedTriangleList(std::vector<T> vertices_in, std::vector<size_t> indices_in)
		:
		vertices(std::move(vertices_in)),
		indices(std::move(indices_in))
	{
		assert(vertices.size() > 2);
		assert(indices.size() % 3 == 0);
		cullFlags.resize(indices.size()/3, false);
	}
	std::vector<T> vertices;
	std::vector<size_t> indices;
	std::vector<bool> cullFlags;
};