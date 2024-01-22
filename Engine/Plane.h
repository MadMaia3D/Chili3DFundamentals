#pragma once
#include "IndexedTriangleList.h"
#include <array>

class Plane {
public:
	template<typename Vertex>
	static IndexedTriangleList<Vertex> GetPlane(float size = 1.0f, int divisions = 8) {
		// divisions is the number of edges at each side of the plane
		// nVertexSide is the number of edges at each side of the plane
		const int nVerticesSide = divisions + 1;
		const float halfSide = size / 2.0f;
		const float segmentSide = size / float(divisions);

		std::vector<Vertex> vertices(sq(nVerticesSide));

		// generate positions
		const Vec3 startPosition = { -halfSide, -halfSide, 0.0f };
		for (int y = 0, i = 0; y < nVerticesSide; y++) {
			for (int x = 0; x < nVerticesSide; x++, i++) {
				Vec3 a = { x * segmentSide, y * segmentSide, 0.0f };
				Vec3 result = startPosition + a;
				vertices[i].pos = result;
			}
		}

		std::vector<size_t> indices;
		indices.reserve(sq(divisions) * 6);
		// generate indices
		const auto xy_to_index = [&nVerticesSide](size_t x, size_t y) { return y * nVerticesSide + x; };
		for (int iy = 0; iy < divisions; iy++) {
			for (int ix = 0; ix < divisions; ix++) {
				std::array<size_t, 4> quadIndices{
					xy_to_index(ix, iy) ,xy_to_index(ix + 1, iy) ,xy_to_index(ix + 1, iy + 1) ,xy_to_index(ix, iy + 1) };
				indices.push_back(quadIndices[0]);
				indices.push_back(quadIndices[2]);
				indices.push_back(quadIndices[1]);
				indices.push_back(quadIndices[0]);
				indices.push_back(quadIndices[3]);
				indices.push_back(quadIndices[2]);
			}
		}
		return IndexedTriangleList<Vertex>(vertices, indices);
	}

	template<typename Vertex>
	static IndexedTriangleList<Vertex> GetSkinnedPlane(float size = 1.0f, int divisions = 8) {
		IndexedTriangleList<Vertex> vertices = GetPlane<Vertex>(size, divisions);
		const int nVerticesSide = divisions + 1;

		// generate coordinates
		const float tcStep = 1.0f / float(divisions);
		for (int y = 0, i = 0; y < nVerticesSide; y++) {
			for (int x = 0; x < nVerticesSide; x++, i++) {
				vertices.vertices[i].tc = { float(x * tcStep), 1.0f - float(y * tcStep) };
			}
		}
		return vertices;
	}

	template<typename Vertex>
	static IndexedTriangleList<Vertex> GetNormalsPlane(float size = 1.0f, int divisions = 8) {
		IndexedTriangleList<Vertex> vertices = GetPlane<Vertex>(size, divisions);
		for (auto& v : vertices.vertices) {
			v.normal = {0.0f, 0.0f, -1.0f};
		}
		return vertices;
	}
};
