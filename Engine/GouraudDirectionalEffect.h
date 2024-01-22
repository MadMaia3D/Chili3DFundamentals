#pragma once
#include "NormalsVertex.h"
#include "GouraudVertexShader.h"
#include "DefaultGeometryShader.h"
#include "PaintByVertexColor.h"

class GouraudDirectionalEffect {
public:
	typedef VertexTypes::NormalsVertex Vertex;
	typedef VertexShaders::GouraudVertexShader<Vertex> VertexShader;
	typedef GeometryShaders::DefaultGeometryShader<VertexShader::Output> GeometryShader;
	typedef PixelShaders::PaintByVertexColor<GeometryShader::Output> PixelShader;
public:
	VertexShader vertexShader;
	GeometryShader geometryShader;
	PixelShader pixelShader;
};