#pragma once
#include "DefaultVertexShader.h"
#include "Surface.h"
#include <algorithm>

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
	typedef DefaultVertexShader<Vertex> VertexShader;
public:
	class PixelShader {
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
public:
	DefaultVertexShader<Vertex> vertexShader;
	PixelShader pixelShader;
};