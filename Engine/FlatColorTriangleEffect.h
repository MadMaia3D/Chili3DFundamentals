#pragma once
#include "PlainPositionVertex.h"
#include "DefaultVertexShader.h"
#include "FlatColorTriangleGeometryShader.h"
#include "PaintByVertexColor.h"

class FlatColorTriangleEffect {
public:
	typedef VertexTypes::PlainPositionVertex Vertex;
	typedef VertexShaders::DefaultVertexShader<Vertex> VertexShader;
	typedef GeometryShaders::FlatColorTriangleGeometryShader<VertexShader::Output> GeometryShader;
	typedef PixelShaders::PaintByVertexColor<GeometryShader::Output> PixelShader;
public:
	VertexShader vertexShader;
	GeometryShader geometryShader;
	PixelShader pixelShader;
};