#pragma once
#include "Vector3.h"
#include "Surface.h"
#include <algorithm>

namespace PixelShaders {
	class FlatTextureEffect {
	public:
		class Vertex {
		public:
			Vertex(const Vec3& position, const Vec2& textureCoordinate)
				:
				pos(position),
				tc(textureCoordinate) {
			}
			Vertex(const Vec3& position, const Vertex& v)
				:
				pos(position),
				tc(v.tc) {
			}
			Vertex& operator+=(const Vertex& rhs) {
				pos += rhs.pos;
				tc += rhs.tc;
				return *this;
			}
			Vertex operator+(const Vertex& rhs) const {
				return Vertex(*this) += rhs;
			}
			Vertex& operator-=(const Vertex& rhs) {
				pos -= rhs.pos;
				tc -= rhs.tc;
				return *this;
			}
			Vertex operator-(const Vertex& rhs) const {
				return Vertex(*this) -= rhs;
			}
			Vertex& operator*=(float rhs) {
				pos *= rhs;
				tc *= rhs;
				return *this;
			}
			Vertex operator*(float rhs) const {
				return Vertex(*this) *= rhs;
			}
			Vertex& operator/=(float rhs) {
				pos /= rhs;
				tc /= rhs;
				return *this;
			}
			Vertex operator/(float rhs) const {
				return Vertex(*this) /= rhs;
			}
			Vec3 pos{};
			Vec2 tc{};
		};
	public:
		Color operator()(const Vertex& scanPosInfo) {
			const int textureLookupX = int(scanPosInfo.tc.x * sufaceMaxWidth);
			const int textureLookupY = int(scanPosInfo.tc.y * surfaceMaxHeight);
			return texture->GetPixel(textureLookupX, textureLookupY);
		}
		void BindTexture(const std::wstring filePath) {
			texture = std::make_unique<Surface>(Surface::FromFile(filePath));
			surfaceWidth = texture->GetWidth();
			surfaceHeight = texture->GetHeight();
			sufaceMaxWidth = surfaceWidth - 1;
			surfaceMaxHeight = surfaceHeight - 1;
		}
	private:
		std::unique_ptr<Surface> texture;
		int surfaceWidth;
		int surfaceHeight;
		int sufaceMaxWidth;
		int surfaceMaxHeight;
	};
}
