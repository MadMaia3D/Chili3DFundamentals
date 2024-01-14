#pragma once
#include "PlainPositionVertex.h"
#include "DefaultVertexShader.h"
#include "Colors.h"
#include <algorithm>

class ZDepthEffect {
public:
	typedef VertexTypes::PlainPositionVertex Vertex;
	typedef VertexShaders::DefaultVertexShader<Vertex> VertexShader;
	class PixelShader {
	public:
		Color operator()(const Vertex& scanPosInfo) const {
			unsigned char value = 255u - unsigned char(std::min(scanPosInfo.pos.z * 30.0f, 254.0f));
			return Color(value, value, value);
		}
	};
public:
	VertexShader vertexShader;
	PixelShader pixelShader;
};