#pragma once
#include "NormalsVertex.h"
#include "ColorVertex.h"
#include "Matrix3.h"
#include "DefaultGeometryShader.h"
#include "PaintByVertexColor.h"
#include <algorithm>

class GouraudPointLightEffect {
public:
	typedef VertexTypes::NormalsVertex Vertex;

	// --------------------------- VERTEX SHADER -------------------------------------

	class VertexShader {
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
			const Vec3 position = input.pos * rotation + translation;
			const Vec3 vertex_to_light = (position - light_position).GetNormalized();
			const float distance_sqr = (position - light_position).LenSquared();

			const Vec3 rotatedVertexNormal = input.normal * rotation;
			const float lightDirectionalInfluence = std::max(0.0f, -(vertex_to_light * rotatedVertexNormal));
			const float lightInfluence = lightDirectionalInfluence * light_intensity / (distance_sqr + 1);
			const Vec3 lightColorInfluence = (light_diffuse_color * lightInfluence + light_ambient_color);
			const Vec3 outputColor = object_color.GetHadamard(lightColorInfluence).Saturate() * 255.0f;
			return { position, outputColor };
		}

		void SetLightPosition(const Vec3& position) {
			light_position = position;
		}
		void SetDiffuseLightColor(const Vec3& color) {
			light_diffuse_color = color;
		}
		void SetLightIntensity(float value) {
			light_intensity = value;
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
		Vec3 light_position = Vec3{ 0.0f, 0.0f, 2.0f };
		float light_intensity = 2.0f;
		Vec3 light_diffuse_color = { 0.8f, 0.7f, 0.5f };
		Vec3 light_ambient_color = { 0.1f, 0.1f, 0.2f };
		Vec3 object_color = { 0.8f, 0.8f, 0.8f };
	};

	// ----------------------------------------------------------------

	typedef GeometryShaders::DefaultGeometryShader<VertexShader::Output> GeometryShader;
	typedef PixelShaders::PaintByVertexColor<GeometryShader::Output> PixelShader;
public:
	VertexShader vertexShader;
	GeometryShader geometryShader;
	PixelShader pixelShader;
};