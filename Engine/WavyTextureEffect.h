#pragma once
#include "TexturedVertex.h"
#include "WavyVertexShader.h"
#include "TextureClipped.h"
#include "DefaultGeometryShader.h"

class WavyTextureEffect {
public:
	typedef VertexTypes::TexturedVertex Vertex;
	typedef VertexShaders::WavyVertexShader<Vertex> VertexShader;
	typedef GeometryShaders::DefaultGeometryShader<VertexShader::Output> GeometryShader;
	typedef PixelShaders::TextureClipped<GeometryShader::Output> PixelShader;
public:
	VertexShader vertexShader;
	GeometryShader geometryShader;
	PixelShader pixelShader;
};