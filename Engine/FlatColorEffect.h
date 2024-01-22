#pragma once
#include "PlainPositionVertex.h"
#include "DefaultVertexShader.h"
#include "DefaultGeometryShader.h"

class FlatColorEffect {
public:
	typedef VertexTypes::PlainPositionVertex Vertex;
	typedef VertexShaders::DefaultVertexShader<Vertex> VertexShader;
	typedef GeometryShaders::DefaultGeometryShader<Vertex> GeometryShader;

	class PixelShader {
	public:
		Color operator()(const Vertex& scanPosInfo) const {
			return color;
		}
		void BindColor(const Color& c) {
			color = c;
		}
	private:
		Color color = Colors::White;
	};

public:
	VertexShader vertexShader;
	GeometryShader geometryShader;
	PixelShader pixelShader;
};