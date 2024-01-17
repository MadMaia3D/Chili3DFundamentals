#pragma once
#include "Matrix3.h"

namespace VertexShaders {
	template <typename Vertex>
	class WavyVertexShader {
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
			pos.z += waveAmplitude * sin((pos.x + pos.y) * waveFrequency + offset * waveSpeed);
			return { pos * rotation + translation, input };
		}
		void OffsetWave(float value) {
			offset += value;
		}
		void SetWaveAmplitude(float value) {
			SetWaveAmplitude = value;
		}
		void SetWaveFrequency(float value) {
			waveFrequency = value;
		}
		void SetWaveSpeed(float value) {
			waveSpeed = value;
		}
	private:
		Vec3 translation;
		Mat3 rotation;
		float waveAmplitude = 0.05f;
		float waveFrequency = 10.0f;
		float waveSpeed = 5.0f;
		float offset = 0.0f;
	};
}