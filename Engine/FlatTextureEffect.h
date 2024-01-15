#pragma once
#include "TexturedVertex.h"
#include "DefaultVertexShader.h"
#include "DefaultGeometryShader.h"
#include "TextureClipped.h"

class FlatTextureEffect {
public:
	typedef VertexTypes::TexturedVertex Vertex;
	typedef VertexShaders::DefaultVertexShader<Vertex> VertexShader;
	typedef GeometryShaders::DefaultGeometryShader<VertexShader::Output> GeometryShader;
	typedef PixelShaders::TextureClipped<GeometryShader::Output> PixelShader;
public:
	VertexShader vertexShader;
	GeometryShader geometryShader;
	PixelShader pixelShader;
};



