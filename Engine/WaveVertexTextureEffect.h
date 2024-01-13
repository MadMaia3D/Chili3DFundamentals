#pragma once
#include "Surface.h"
#include "Matrix3.h"
#include <algorithm>

class WaveVertexTextureEffect {
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

	class VertexShader {
	public:
		typedef Vertex Output;
	public:
		void BindRotation(const Mat3& rotation_in) {
			rotation = rotation_in;
		}
		void BindTranslation(const Vec3& translation_in) {
			translation = translation_in;
		}
		Output operator()(const Vertex& input) {
			Vec3 pos = input.pos * rotation + translation;
			pos.y += waveAmplitude * sin( pos.x * waveFrequency + offset * waveSpeed );
			return { pos, input };
		}
		void SetWaveOffset(float value) {
			offset += value;
		}
	private:
		Vec3 translation;
		Mat3 rotation;
		float waveAmplitude = 0.05f;
		float waveFrequency = 20.0f;
		float waveSpeed = 2.0f;
		float offset = 0.0f;
	};
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
	VertexShader vertexShader;
	PixelShader pixelShader;
};