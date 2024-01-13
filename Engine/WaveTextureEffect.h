#pragma once
#include "Surface.h"
#include "Matrix3.h"
#include <algorithm>
#include "VertexTypes.h"
#include "PixelShaders.h"

class WaveTextureEffect {
public:
	typedef TexturedVertex Vertex;
	typedef PixelShaders::TextureClipped<Vertex> PixelShader;

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
			Vec3 pos = input.pos;
			pos.z += waveAmplitude * sin( (pos.x + pos.y) * waveFrequency + offset * waveSpeed );
			return { pos * rotation + translation, input };
		}
		void SetWaveOffset(float value) {
			offset += value;
		}
	private:
		Vec3 translation;
		Mat3 rotation;
		float waveAmplitude = 0.05f;
		float waveFrequency = 10.0f;
		float waveSpeed = 5.0f;
		float offset = 0.0f;
	};
public:
	VertexShader vertexShader;
	PixelShader pixelShader;
};