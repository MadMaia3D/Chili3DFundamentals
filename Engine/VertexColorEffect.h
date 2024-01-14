#pragma once
#include "ColorVertex.h"
#include "DefaultVertexShader.h"
#include "PaintByVertexColor.h"

class VertexColorEffect{
public:	
	typedef VertexTypes::ColorVertex Vertex;
	typedef VertexShaders::DefaultVertexShader<Vertex> VertexShader;
	typedef PixelShaders::PaintByVertexColor<Vertex> PixelShader;	
public:
	VertexShader vertexShader;
	PixelShader pixelShader;
};