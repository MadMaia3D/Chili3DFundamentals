#pragma once
#include "Matrix3.h"
#include "ColorVertex.h"

namespace VertexShaders {
	template <class Vertex>
	class GouraudVertexShader {
	public:
		typedef VertexTypes::ColorVertex Output;
	public:
		void BindRotation(const Mat3& rotation_in) {
			rotation = rotation_in;
		}
		void BindTranslation(const Vec3& translation_in) {
			translation = translation_in;
		}

		Output operator()(const Vertex& input) const {
			// light calculations
			const Vec3 rotatedNormals = input.normal * rotation;
			const float lightInfluence = std::max(0.0f, -(light_direction * rotatedNormals));
			const Vec3 lightColorInfluence = (light_diffuse_color * lightInfluence + light_ambient_color);
			const Vec3 outputColor = object_color.GetHadamard(lightColorInfluence).Saturate() * 255.0f;
			return { input.pos * rotation + translation, outputColor };
		}

		void SetLightDirection(const Vec3& direction) {
			light_direction = direction;
		}
		void SetDiffuseLightColor(const Vec3& color) {
			light_diffuse_color = light;
		}
		void SetAmbientLightColor(const Vec3& color) {
			light_ambient_color = color;
		}
		void SetObjectColor(const Vec3& color) {
			object_color = color;
		}
	private:
		Vec3 translation;
		Mat3 rotation;
		Vec3 light_direction = Vec3{ 0.5f, 0.0f, 1.0f }.GetNormalized();
		Vec3 light_diffuse_color = {0.8f, 0.7f, 0.5f};
		Vec3 light_ambient_color = { 0.1f, 0.1f, 0.2f };
		Vec3 object_color = { 0.8f, 0.8f, 0.8f };
	};
}
