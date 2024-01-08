#pragma once
#include "IndexedTriangleList.h"
#include "TexVertex.h"
#include "Triangle.h"
#include "PerspectiveScreenTransformer.h"
#include "Matrix3.h"
#include "ChiliMath.h"
#include "FlatTextureEffect.h"

template <class PixelShaderEffect>
class Pipeline {
public:
	typedef typename PixelShaderEffect::Vertex Vertex;
public:
	Pipeline(Graphics& gfx)
		:
		gfx(gfx) {
	}

	void Draw(const IndexedTriangleList<TexVertex>& vertices) {
		VertexTransform(vertices);
	}

	void BindRotation(const Mat3& rotation_in) { rotation = rotation_in; }
	void BindTranslation(const Vec3& translation_in) { translation = translation_in; }

private:
	void VertexTransform(const IndexedTriangleList<TexVertex>& IndexedTriangle) {
		// create a transformed copy of the vertices
		std::vector<Vertex> worldSpaceVertices;
		worldSpaceVertices.reserve(IndexedTriangle.vertices.size());
		for (const auto &v : IndexedTriangle.vertices) {
			worldSpaceVertices.emplace_back(v.pos * rotation + translation, v.tc);
		}
		// call assemble triangle with the transformed vertices and its indexes
		AssembleTriangles(worldSpaceVertices, IndexedTriangle.indices);
	}

	void AssembleTriangles(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices) {
		// loop through indices getting 3 at each time and creating copies of the vertices
		for (size_t i = 0; i < indices.size() / 3; i++) {
			const Vertex& v0 = vertices[indices[i * 3]];
			const Vertex& v1 = vertices[indices[i * 3 + 1]];
			const Vertex& v2 = vertices[indices[i * 3 + 2]];
			// test each vertice trio for back face culling
			if ((v1.pos - v0.pos).CrossProduct(v2.pos - v0.pos) * v0.pos <= 0) {
				PerspectiveScreenTransform(v0, v1, v2);
			};
		}
	}

	void PerspectiveScreenTransform(Vertex v0, Vertex v1, Vertex v2) {
		// apply perspective transform to the triangle
		// apply screen transform to the triangle
		pst.Transform(v0.pos);
		pst.Transform(v1.pos);
		pst.Transform(v2.pos);
		RasterizeTriangle(v0, v1, v2);
	}

	void RasterizeTriangle(Vertex& v0, Vertex& v1, Vertex& v2) {
		DrawTriangle(v0, v1, v2);
	}

	void DrawTriangle(const Vertex & v0, const Vertex & v1, const Vertex & v2) {
		const Vertex* pv0 = &v0;
		const Vertex* pv1 = &v1;
		const Vertex* pv2 = &v2;

		if (pv1->pos.y < pv0->pos.y) { std::swap(pv0, pv1); }
		if (pv2->pos.y < pv1->pos.y) { std::swap(pv1, pv2); }
		if (pv1->pos.y < pv0->pos.y) { std::swap(pv0, pv1); }

		// for each case, points should be ordered in clockwise direction, starting from the most top-left point
		if (pv0->pos.y == pv1->pos.y) {									// render flat top triangle
			if (pv1->pos.x < pv0->pos.x) { std::swap(pv0, pv1); }
			DrawFlatTopTriangle(*pv0, *pv1, *pv2);
		} else if (pv1->pos.y == pv2->pos.y) {							// render flat bottom triangle
			if (pv1->pos.x < pv2->pos.x) { std::swap(pv1, pv2); }
			DrawFlatBottomTriangle(*pv0, *pv1, *pv2);
		} else {												// render separated triangles
			// find alpha for linear interpolation (find the percentage of middle y between bottom y and top y)
			const float alpha = (pv1->pos.y - pv0->pos.y) / (pv2->pos.y - pv0->pos.y);
			// split vertex is the linear interpolation between v0 and v1
			const Vertex sv = Interpolate(*pv0, *pv2, alpha);

			if (sv.pos.x < pv1->pos.x) {
				DrawFlatBottomTriangle(*pv0, *pv1, sv);
				DrawFlatTopTriangle(sv, *pv1, *pv2);
			} else {
				DrawFlatBottomTriangle(*pv0, sv, *pv1);
				DrawFlatTopTriangle(*pv1, sv, *pv2);
			}
		}
	}

	// mind the order of vertices: clockwise from the top
	void DrawFlatBottomTriangle(const Vertex & v0, const Vertex & v1, const Vertex & v2) {
		// calculate step of left and right edges
		const float deltaY = (v2.pos.y - v0.pos.y);
		Vertex leftEdgeStep = (v2 - v0) / deltaY;
		Vertex rightEdgeStep = (v1 - v0) / deltaY;
		DrawFlatTriangle(v0, v1, v2, leftEdgeStep, rightEdgeStep, v0, v0);
	}

	// mind the order of vertices: clockwise from the top left
	void DrawFlatTopTriangle(const Vertex & v0, const Vertex & v1, const Vertex & v2) {
		// calculate step of left and right edges
		const float deltaY = (v2.pos.y - v0.pos.y);
		Vertex leftEdgeStep = (v2 - v0) / deltaY;
		Vertex rightEdgeStep = (v2 - v1) / deltaY;
		DrawFlatTriangle(v0, v1, v2, leftEdgeStep, rightEdgeStep, v0, v1);
	}

	void DrawFlatTriangle(const Vertex & v0,
						  const Vertex & v1,
						  const Vertex & v2,
						  const Vertex& leftEdgestep,
						  const Vertex& rightEdgestep,
						  Vertex leftEdgeStartVertex,
						  Vertex rightEdgeStartVertex) {
		// Microsoft DirectX10 rasterization "top-edge" rule
		const int yStart = (int)std::ceil(v0.pos.y - 0.5f);
		const int yEnd = (int)std::ceil(v2.pos.y - 0.5f);

		// do pre step
		Vertex&leftEdgeInterpolant = leftEdgeStartVertex;
		Vertex& rightEdgeInterpolant = rightEdgeStartVertex;
		leftEdgeInterpolant += leftEdgestep * (float(yStart) - leftEdgeInterpolant.pos.y + 0.5f);
		rightEdgeInterpolant += rightEdgestep * (float(yStart) - rightEdgeInterpolant.pos.y + 0.5f);

		for (int y = yStart; y < yEnd; y++,
			 leftEdgeInterpolant += leftEdgestep, rightEdgeInterpolant += rightEdgestep) {
			// following Microsoft DirectX10 rasterization "left-edge" rule
			const int xStart = (int)std::ceil(leftEdgeInterpolant.pos.x - 0.5f);
			const int xEnd = (int)std::ceil(rightEdgeInterpolant.pos.x - 0.5f);

			Vertex scanPos = leftEdgeInterpolant;
			// calculate scanline unit step
			const float deltaX = rightEdgeInterpolant.pos.x - leftEdgeInterpolant.pos.x;
			Vertex scanPosDelta = (rightEdgeInterpolant - leftEdgeInterpolant) / deltaX;

			for (int x = xStart; x < xEnd; x++, scanPos += scanPosDelta) {
				gfx.PutPixel(x, y, pixelShader(scanPos));
			}
		}
	}
public:
	PixelShaderEffect pixelShader;
private:
	Graphics& gfx;
	PerspectiveScreenTransformer pst;
	Mat3 rotation;
	Vec3 translation;
};