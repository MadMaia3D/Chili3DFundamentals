#pragma once
#include "IndexedTriangleList.h"
#include "TexVertex.h"
#include "Triangle.h"
#include "PerspectiveScreenTransformer.h"
#include "Matrix3.h"
#include "ChiliMath.h"
#include "ZBuffer.h"
#include <algorithm>
#include <iterator>
#include <Windows.h>

template <class Effect>
class Pipeline {
public:
	typedef typename Effect::Vertex Vertex;
	typedef typename Effect::VertexShader::Output VSOut;
public:
	Pipeline(Graphics& gfx)
		:
		gfx(gfx),
		buffer(gfx.ScreenWidth, gfx.ScreenHeight)
	{
	}

	void Draw(const IndexedTriangleList<Vertex>& vertices) {
		VertexTransform(vertices);
	}
	void BeginFrame() {
		buffer.Clear();
	}

private:
	void VertexTransform(const IndexedTriangleList<Vertex>& IndexedTriangles) {
		// create a transformed copy of the vertices
		std::vector<VSOut> worldSpaceVertices;

		std::transform(IndexedTriangles.vertices.begin() ,
					   IndexedTriangles.vertices.end(),
					   std::back_inserter(worldSpaceVertices),
					   effect.vertexShader);

		// call assemble triangle with the transformed vertices and its indexes
		AssembleTriangles(worldSpaceVertices, IndexedTriangles.indices);
	}

	void AssembleTriangles(const std::vector<VSOut>& vertices, const std::vector<size_t>& indices) {
		// loop through indices getting 3 at each time and creating copies of the vertices
		for (size_t i = 0; i < indices.size() / 3; i++) {
			const VSOut& v0 = vertices[indices[i * 3]];
			const VSOut& v1 = vertices[indices[i * 3 + 1]];
			const VSOut& v2 = vertices[indices[i * 3 + 2]];
			// test each vertice trio for back face culling
			if ((v1.pos - v0.pos).CrossProduct(v2.pos - v0.pos) * v0.pos <= 0) {
				PerspectiveScreenTransform(v0, v1, v2);
			};
		}
	}

	void PerspectiveScreenTransform(VSOut v0, VSOut v1, VSOut v2) {
		// apply perspective transform to the triangle
		// apply screen transform to the triangle
		pst.TransformVertex(v0);
		pst.TransformVertex(v1);
		pst.TransformVertex(v2);
		RasterizeTriangle(v0, v1, v2);
	}

	void RasterizeTriangle(VSOut& v0, VSOut& v1, VSOut& v2) {
		DrawTriangle(v0, v1, v2);
	}

	void DrawTriangle(const VSOut & v0, const VSOut & v1, const VSOut & v2) {
		const VSOut* pv0 = &v0;
		const VSOut* pv1 = &v1;
		const VSOut* pv2 = &v2;

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
			const VSOut sv = Interpolate(*pv0, *pv2, alpha);

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
	void DrawFlatBottomTriangle(const VSOut & v0, const VSOut & v1, const VSOut & v2) {
		// calculate step of left and right edges
		const float deltaY = (v2.pos.y - v0.pos.y);
		VSOut leftEdgeStep = (v2 - v0) / deltaY;
		VSOut rightEdgeStep = (v1 - v0) / deltaY;
		DrawFlatTriangle(v0, v1, v2, leftEdgeStep, rightEdgeStep, v0, v0);
	}

	// mind the order of vertices: clockwise from the top left
	void DrawFlatTopTriangle(const VSOut & v0, const VSOut & v1, const VSOut & v2) {
		// calculate step of left and right edges
		const float deltaY = (v2.pos.y - v0.pos.y);
		VSOut leftEdgeStep = (v2 - v0) / deltaY;
		VSOut rightEdgeStep = (v2 - v1) / deltaY;
		DrawFlatTriangle(v0, v1, v2, leftEdgeStep, rightEdgeStep, v0, v1);
	}

	void DrawFlatTriangle(const VSOut & v0,
						  const VSOut & v1,
						  const VSOut & v2,
						  const VSOut& leftEdgestep,
						  const VSOut& rightEdgestep,
						  VSOut leftEdgeStartVertex,
						  VSOut rightEdgeStartVertex) {
		// Microsoft DirectX10 rasterization "top-edge" rule
		const int yStart = (int)std::ceil(v0.pos.y - 0.5f);
		const int yEnd = (int)std::ceil(v2.pos.y - 0.5f);
		// do pre step
		VSOut&leftEdgeInterpolant = leftEdgeStartVertex;
		VSOut& rightEdgeInterpolant = rightEdgeStartVertex;
		leftEdgeInterpolant += leftEdgestep * (float(yStart) - leftEdgeInterpolant.pos.y + 0.5f);
		rightEdgeInterpolant += rightEdgestep * (float(yStart) - rightEdgeInterpolant.pos.y + 0.5f);

		for (int y = yStart; y < yEnd; y++,
			 leftEdgeInterpolant += leftEdgestep, rightEdgeInterpolant += rightEdgestep) {
			// following Microsoft DirectX10 rasterization "left-edge" rule
			const int xStart = (int)std::ceil(leftEdgeInterpolant.pos.x - 0.5f);
			const int xEnd = (int)std::ceil(rightEdgeInterpolant.pos.x - 0.5f);

			VSOut scanPos = leftEdgeInterpolant;
			// calculate scanline unit step
			const float deltaX = rightEdgeInterpolant.pos.x - leftEdgeInterpolant.pos.x;
			VSOut scanPosDelta = (rightEdgeInterpolant - leftEdgeInterpolant) / deltaX;

			for (int x = xStart; x < xEnd; x++, scanPos += scanPosDelta) {
				const float uninvertedZ = 1.0f / scanPos.pos.z;
				VSOut attributes = scanPos * uninvertedZ;
				if (buffer.TestAndSet(x,y, uninvertedZ)) {
					gfx.PutPixel(x, y, effect.pixelShader(attributes));
				}
			}
		}
	}
public:
	Effect effect;
private:
	Graphics& gfx;
	ZBuffer buffer;
	PerspectiveScreenTransformer pst;
};