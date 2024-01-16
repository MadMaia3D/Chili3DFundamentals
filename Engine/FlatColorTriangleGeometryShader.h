#pragma once
#include "Triangle.h"
#include "ColorVertex.h"
#include <vector>

namespace GeometryShaders {
	template <typename Vertex>
	class FlatColorTriangleGeometryShader {
	public:
		typedef VertexTypes::ColorVertex Output;
	public:
		void BindColors(std::vector<Vec3> colors) {
			triangle_colors = std::move(colors);
		}
		Triangle<Output> operator()(const Vertex& v0, const Vertex& v1, const Vertex& v2, size_t index) {
			index %= triangle_colors.size();
			return { {v0.pos, triangle_colors[index]}, {v1.pos, triangle_colors[index]}, {v2.pos, triangle_colors[index]} };
		}
	private:
		std::vector<Vec3> triangle_colors;
	};
}