#pragma once
#include "DefaultVertexShader.h"
#include "Surface.h"
#include <algorithm>
#include "VertexTypes.h"
#include "PixelShaders.h"

class FlatTextureEffect {
public:
	typedef TexturedVertex Vertex;
	typedef DefaultVertexShader<Vertex> VertexShader;
	typedef PixelShaders::TextureClipped<Vertex> PixelShader;
public:
	DefaultVertexShader<Vertex> vertexShader;
	PixelShader pixelShader;
};