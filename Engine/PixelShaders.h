#pragma once
#include <memory>

namespace PixelShaders {
	template <typename Vertex>
	class TextureClipped {
	public:
		Color operator()(const Vertex& scanPosInfo) const {
			assert(texture.get() != nullptr);
			const int textureLookupX = std::clamp(int(scanPosInfo.tc.x * sufaceMaxWidth), 0, sufaceMaxWidth);
			const int textureLookupY = std::clamp(int(scanPosInfo.tc.y * surfaceMaxHeight), 0, surfaceMaxHeight);
			return texture->GetPixel(textureLookupX, textureLookupY);
		}
		void BindTexture(const std::wstring filePath) {
			texture = std::make_unique<Surface>(Surface::FromFile(filePath));
			const int surfaceWidth = texture->GetWidth();
			const int surfaceHeight = texture->GetHeight();
			sufaceMaxWidth = surfaceWidth - 1;
			surfaceMaxHeight = surfaceHeight - 1;
		}
	private:
		std::unique_ptr<Surface> texture;
		int sufaceMaxWidth;
		int surfaceMaxHeight;
	};
}