#pragma once
#include "PlainPositionVertex.h"
#include "DefaultVertexShader.h"
#include "FlatLightingGeometryShader.h"
#include "PaintByVertexColor.h"

class FlatLightingEffect {
public:
	typedef VertexTypes::PlainPositionVertex Vertex;
	typedef VertexShaders::DefaultVertexShader<Vertex> VertexShader;
	typedef GeometryShaders::FlatLightingGeometryShader<VertexShader::Output> GeometryShader;
	typedef PixelShaders::PaintByVertexColor<GeometryShader::Output> PixelShader;
public:
	VertexShader vertexShader;
	GeometryShader geometryShader;
	PixelShader pixelShader;
};