#pragma once
#include "IndexedTriangleList.h"
#include "TexVertex.h"
#include "Triangle.h"
#include "PerspectiveScreenTransformer.h"
#include "Matrix3.h"
#include "ChiliMath.h"

class Pipeline {
public:
	class Vertex {
	public:
		Vertex(const Vec3& position, const Vec2& textureCoordinate)
			:
			pos(position),
			tc(textureCoordinate) {
		}
		Vertex(const TexVertex& texV)
			:
			pos(texV.pos), tc(texV.tc) {
		}
		Vertex& operator+=(const Vertex& rhs) {
			pos += rhs.pos;
			tc += rhs.tc;
			return *this;
		}
		Vertex operator+(const Vertex& rhs) {
			return Vertex(*this) += rhs;
		}
		Vertex& operator-=(const Vertex& rhs) {
			pos -= rhs.pos;
			tc -= rhs.tc;
			return *this;
		}
		Vertex operator-(const Vertex& rhs) {
			return Vertex(*this) -= rhs;
		}
		Vertex& operator*=(float rhs) {
			pos *= rhs;
			tc *= rhs;
			return *this;
		}
		Vertex operator*(float rhs) {
			return Vertex(*this) *= rhs;
		}
		Vertex& operator/=(float rhs) {
			pos /= rhs;
			tc /= rhs;
			return *this;
		}
		Vertex operator/(float rhs) {
			return Vertex(*this) /= rhs;
		}
		Vec3 pos{};
		Vec2 tc{};
	};
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
	void BindTexture(const Surface* surf) { texture = surf; };

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
		DrawTriangle(v0, v1, v2, *texture);
	}

	void DrawTriangle(const Vertex & v0, const Vertex & v1, const Vertex & v2, const Surface& surf) {
		const Vertex* pv0 = &v0;
		const Vertex* pv1 = &v1;
		const Vertex* pv2 = &v2;

		if (pv1->pos.y < pv0->pos.y) { std::swap(pv0, pv1); }
		if (pv2->pos.y < pv1->pos.y) { std::swap(pv1, pv2); }
		if (pv1->pos.y < pv0->pos.y) { std::swap(pv0, pv1); }

		// for each case, points should be ordered in clockwise direction, starting from the most top-left point
		if (pv0->pos.y == pv1->pos.y) {									// render flat top triangle
			if (pv1->pos.x < pv0->pos.x) { std::swap(pv0, pv1); }
			DrawFlatTopTriangle(*pv0, *pv1, *pv2, surf);
		} else if (pv1->pos.y == pv2->pos.y) {							// render flat bottom triangle
			if (pv1->pos.x < pv2->pos.x) { std::swap(pv1, pv2); }
			DrawFlatBottomTriangle(*pv0, *pv1, *pv2, surf);
		} else {												// render separated triangles
			// find alpha for linear interpolation (find the percentage of middle y between bottom y and top y)
			const float alpha = (pv1->pos.y - pv0->pos.y) / (pv2->pos.y - pv0->pos.y);
			// split vertex is the linear interpolation between v0 and v1
			const Vertex sv = Interpolate(*pv0, *pv2, alpha);

			if (sv.pos.x < pv1->pos.x) {
				DrawFlatBottomTriangle(*pv0, *pv1, sv, surf);
				DrawFlatTopTriangle(sv, *pv1, *pv2, surf);
			} else {
				DrawFlatBottomTriangle(*pv0, sv, *pv1, surf);
				DrawFlatTopTriangle(*pv1, sv, *pv2, surf);
			}
		}
	}

	// mind the order of vertices: clockwise from the top
	void DrawFlatBottomTriangle(const Vertex & v0, const Vertex & v1, const Vertex & v2, const Surface& surf) {
		// use linear equation (y = mx +b) but the version of x in function of y (x = wy + p),
		// where w is the run over the rise (inverse of m) and p is the x-intercept (the original b is the y-intercept)
		// "y in function of x" would cause problems with vertical lines since 'm' in that case would result in division by zero
		// so we use "x in function of x" version cause the lines that we won't need to calculate the bottom horizontal edge of the triangle

		// run over rise for left and right sides
		const float deltaY = (v2.pos.y - v0.pos.y);
		const float lw = (v2.pos.x - v0.pos.x) / deltaY;
		const float rw = (v1.pos.x - v0.pos.x) / deltaY;

		// following Microsoft DirectX10 rasterization "top-edge" rule
		const int yStart = (int)std::ceil(v0.pos.y - 0.5f);
		const int yEnd = (int)std::ceil(v2.pos.y - 0.5f);

		// init text coord edges
		Vec2 tcEdgeL = v0.tc;
		Vec2 tcEdgeR = v0.tc;

		// calculate tex coord edge unit steps
		const Vec2 tcEdgeStepL = (v2.tc - v0.tc) / (v2.pos.y - v0.pos.y);
		const Vec2 tcEdgeStepR = (v1.tc - v0.tc) / (v1.pos.y - v0.pos.y);

		// do tex coord pre step
		tcEdgeL += tcEdgeStepL * ((float)yStart - v0.pos.y + 0.5f);
		tcEdgeR += tcEdgeStepR * ((float)yStart - v0.pos.y + 0.5f);

		// get texture info
		const int surfaceWidth = surf.GetWidth();
		const int surfaceHeight = surf.GetHeight();
		const int sufaceMaxWidth = surfaceWidth - 1;
		const int surfaceMaxHeight = surfaceHeight - 1;

		for (int y = yStart; y < yEnd; y++,
			 tcEdgeL += tcEdgeStepL, tcEdgeR += tcEdgeStepR) {
			// add 0.5 to y value because we are making calculations based on the pixel center
			const float px0 = lw * (y + 0.5f - v0.pos.y) + v0.pos.x;
			const float px1 = rw * (y + 0.5f - v0.pos.y) + v0.pos.x;
			// following Microsoft DirectX10 rasterization "left-edge" rule
			const int xStart = (int)std::ceil(px0 - 0.5f);
			const int xEnd = (int)std::ceil(px1 - 0.5f);

			// calculate text coord scanline unit step
			const Vec2 tcScanStep = (tcEdgeR - tcEdgeL) / (px1 - px0);
			Vec2 tcScanPos = tcEdgeL + tcScanStep * ((float)xStart - px0 + 0.5f);

			for (int x = xStart; x < xEnd; x++, tcScanPos += tcScanStep) {
				const int surfaceX = std::min(int(tcScanPos.x * surfaceWidth), sufaceMaxWidth);
				const int surfaceY = std::min(int(tcScanPos.y * surfaceHeight), surfaceMaxHeight);
				gfx.PutPixel(x, y, surf.GetPixel(surfaceX, surfaceY));
			}
		}
	}

	// mind the order of vertices: clockwise from the top left
	void DrawFlatTopTriangle(const Vertex & v0, const Vertex & v1, const Vertex & v2, const Surface& surf) {
		// use linear equation (y = mx +b) but the version of x in function of y (x = wy + p),
		// where w is the run over the rise (inverse of m) and p is the x-intercept (the original b is the y-intercept)
		// "y in function of x" would cause problems with vertical lines since 'm' in that case would result in division by zero
		// so we use "x in function of x" version cause the lines that we won't need to calculate the top horizontal edge of the triangle

		// run over rise for left and right sides
		const float deltaY = (v2.pos.y - v0.pos.y);
		const float lw = (v2.pos.x - v0.pos.x) / deltaY;
		const float rw = (v2.pos.x - v1.pos.x) / deltaY;

		// following Microsoft DirectX10 rasterization "top-edge" rule
		const int yStart = (int)std::ceil(v0.pos.y - 0.5f);
		const int yEnd = (int)std::ceil(v2.pos.y - 0.5f);

		// init text coord edges
		Vec2 tcEdgeL = v0.tc;
		Vec2 tcEdgeR = v1.tc;

		// calculate tex coord edge unit steps
		const Vec2 tcEdgeStepL = (v2.tc - v0.tc) / (v2.pos.y - v0.pos.y);
		const Vec2 tcEdgeStepR = (v2.tc - v1.tc) / (v2.pos.y - v1.pos.y);

		// do tex coord pre step
		tcEdgeL += tcEdgeStepL * (float(yStart) - v0.pos.y + 0.5f);
		tcEdgeR += tcEdgeStepR * (float(yStart) - v1.pos.y + 0.5f);

		for (int y = yStart; y < yEnd; y++,
			 tcEdgeL += tcEdgeStepL, tcEdgeR += tcEdgeStepR) {
			// add 0.5 to y value because we are making calculations based on the pixel center
			const float px0 = lw * (y + 0.5f - v0.pos.y) + v0.pos.x;
			const float px1 = rw * (y + 0.5f - v1.pos.y) + v1.pos.x;
			// following Microsoft DirectX10 rasterization "left-edge" rule
			const int xStart = (int)std::ceil(px0 - 0.5f);
			const int xEnd = (int)std::ceil(px1 - 0.5f);

			// calculate text coord scanline unit step
			const Vec2 tcScanStep = (tcEdgeR - tcEdgeL) / (px1 - px0);
			Vec2 tcScanPos = tcEdgeL + tcScanStep * ((float)xStart - px0 + 0.5f);

			// get texture info
			const int surfaceWidth = surf.GetWidth();
			const int surfaceHeight = surf.GetHeight();
			const int sufaceMaxWidth = surfaceWidth - 1;
			const int surfaceMaxHeight = surfaceHeight - 1;

			for (int x = xStart; x < xEnd; x++, tcScanPos += tcScanStep) {
				const int surfaceX = std::min(int(tcScanPos.x * surfaceWidth), sufaceMaxWidth);
				const int surfaceY = std::min(int(tcScanPos.y * surfaceHeight), surfaceMaxHeight);
				gfx.PutPixel(x, y, surf.GetPixel(surfaceX, surfaceY));
			}
		}
	}

private:
	Graphics& gfx;
	PerspectiveScreenTransformer pst;
	Mat3 rotation;
	Vec3 translation;
	const Surface* texture;
};