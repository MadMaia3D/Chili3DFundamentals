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

	template<typename Vertex>
	static IndexedTriangleList<Vertex> LoadMeshWithNormals(std::string file_name) {
		const ObjParser::Model model = ObjParser::GetParsed(file_name);

		const size_t totalTriangles = model.faces.size();
		std::vector<Vertex> vertices(totalTriangles * 3);
		std::vector<size_t> indices(totalTriangles * 3);

		for (size_t i = 0; i < totalTriangles; i++) {
			const int v0pos = model.faces[i][0].v - 1;
			const int v1pos = model.faces[i][1].v - 1;
			const int v2pos = model.faces[i][2].v - 1;
			vertices[i * 3].pos = { -model.positions[v0pos].x, model.positions[v0pos].y , model.positions[v0pos].z };
			vertices[i * 3 + 1].pos = { -model.positions[v1pos].x, model.positions[v1pos].y , model.positions[v1pos].z };
			vertices[i * 3 + 2].pos = { -model.positions[v2pos].x, model.positions[v2pos].y , model.positions[v2pos].z };
			const int v0norm = model.faces[i][0].vn - 1;
			const int v1norm = model.faces[i][1].vn - 1;
			const int v2norm = model.faces[i][2].vn - 1;
			vertices[i * 3].normal = { -model.normals[v0norm].x, model.normals[v0norm].y , model.normals[v0norm].z };
			vertices[i * 3 + 1].normal = { -model.normals[v1norm].x, model.normals[v1norm].y , model.normals[v1norm].z };
			vertices[i * 3 + 2].normal = { -model.normals[v2norm].x, model.normals[v2norm].y , model.normals[v2norm].z };
		}

		for (size_t i = 0; i < totalTriangles; i++) {
			indices[i * 3] = i * 3 + 2;
			indices[i * 3 + 1] = i * 3 + 1;
			indices[i * 3 + 2] = i * 3;
		}

		return { std::move(vertices),std::move(indices) };
	}
};