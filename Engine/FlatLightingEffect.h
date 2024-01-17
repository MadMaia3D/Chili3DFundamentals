#pragma once
#include "NormalsVertex.h"
#include "FlatLightingVertexShader.h"
#include "DefaultGeometryShader.h"
#include "PaintByVertexColor.h"

class FlatLightingEffect {
public:
	typedef VertexTypes::NormalsVertex Vertex;
	typedef VertexShaders::FlatLightingVertexShader<Vertex> VertexShader;
	typedef GeometryShaders::DefaultGeometryShader<VertexShader::Output> GeometryShader;
	typedef PixelShaders::PaintByVertexColor<GeometryShader::Output> PixelShader;
public:
	VertexShader vertexShader;
	GeometryShader geometryShader;
	PixelShader pixelShader;
};