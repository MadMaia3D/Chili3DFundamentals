#pragma once
#include "IndexedTriangleList.h"
#include <array>
#include "ObjParser.h"

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

	template<typename Vertex>
	static IndexedTriangleList<Vertex> LoadPlainMesh(std::string file_name) {
		const ObjParser::Model model = ObjParser::GetParsed(file_name);

		const size_t totalPositions = model.positions.size();
		std::vector<Vertex> vertices(totalPositions);

		const size_t totalTriangles = model.faces.size();
		std::vector<size_t> indices(totalTriangles * 3);

		for (size_t i = 0; i < totalPositions; i++) {
			vertices[i].pos = { -model.positions[i].x, model.positions[i].y , model.positions[i].z };
		}

		for (size_t i = 0; i < totalTriangles; i++) {
			indices[i * 3    ] = model.faces[i][2].v - 1;
			indices[i * 3 + 1] = model.faces[i][1].v - 1;
			indices[i * 3 + 2] = model.faces[i][0].v - 1;
		}
		return { std::move(vertices),std::move(indices) };
	}
};