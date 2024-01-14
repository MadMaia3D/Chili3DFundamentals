#pragma once
#include "TexturedVertex.h"
#include "DefaultVertexShader.h"
#include "TextureClipped.h"

class FlatTextureEffect {
public:
	typedef VertexTypes::TexturedVertex Vertex;
	typedef VertexShaders::DefaultVertexShader<Vertex> VertexShader;
	typedef PixelShaders::TextureClipped<Vertex> PixelShader;
public:
	VertexShader vertexShader;
	PixelShader pixelShader;
};



