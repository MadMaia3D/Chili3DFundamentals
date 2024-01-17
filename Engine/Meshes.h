#pragma once
#include "IndexedTriangleList.h"
#include <array>

class Meshes {
public:
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
};