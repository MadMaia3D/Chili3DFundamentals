#pragma once
#include "TexturedVertex.h"
#include "WavyVertexShader.h"
#include "TextureClipped.h"

class WavyTextureEffect {
public:
	typedef VertexTypes::TexturedVertex Vertex;
	typedef VertexShaders::WavyVertexShader<Vertex> VertexShader;
	typedef PixelShaders::TextureClipped<Vertex> PixelShader;
public:
	VertexShader vertexShader;
	PixelShader pixelShader;
};