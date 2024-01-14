#pragma once
#include "Colors.h"

namespace PixelShaders {
	template <typename Vertex>
	class PaintByVertexColor {
	public:
		Color operator()(const Vertex& scanPosInfo) const {
			return Color(scanPosInfo.color);
		}
	};
}
