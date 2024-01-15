#pragma once
#include "ColorVertex.h"
#include "DefaultVertexShader.h"
#include "DefaultGeometryShader.h"
#include "PaintByVertexColor.h"

class VertexColorEffect{
public:	
	typedef VertexTypes::ColorVertex Vertex;
	typedef VertexShaders::DefaultVertexShader<Vertex> VertexShader;
	typedef GeometryShaders::DefaultGeometryShader<VertexShader::Output> GeometryShader;
	typedef PixelShaders::PaintByVertexColor<GeometryShader::Output> PixelShader;
public:
	VertexShader vertexShader;
	GeometryShader geometryShader;
	PixelShader pixelShader;
};