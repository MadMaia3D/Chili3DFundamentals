#pragma once
#include "PlainPositionVertex.h"
#include "WavyVertexShader.h"
#include "FlatLightingGeometryShader.h"
#include "PaintByVertexColor.h"

class WavyFlatLightingEffect {
public:
	typedef VertexTypes::PlainPositionVertex Vertex;
	typedef VertexShaders::WavyVertexShader<Vertex> VertexShader;
	typedef GeometryShaders::FlatLightingGeometryShader<VertexShader::Output> GeometryShader;
	typedef PixelShaders::PaintByVertexColor<GeometryShader::Output> PixelShader;
public:
	VertexShader vertexShader;
	GeometryShader geometryShader;
	PixelShader pixelShader;
};