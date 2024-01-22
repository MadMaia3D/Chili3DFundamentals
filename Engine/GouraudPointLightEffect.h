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
			// transform each vertex before light calc
			const Vec3 position = input.pos * rotation + translation;
			// vertex to light data
			const Vec3 v_to_l = light_position - position;
			const float dist = v_to_l.Lenght();
			const Vec3 dir = v_to_l / dist;
			// calculate attenuation
			const float attenuation = 1.0f /
				(quadratic_attenuation * sq(dist) + linear_attenuation * dist + constant_attenuation);
			// calculate intensity based on angle of incidence and attenuation
			const float angle_incidence = std::max(0.0f, (dir * (input.normal * rotation)));
			const float lightInfluence = light_intensity * angle_incidence * attenuation;

			const Vec3 lightColorInfluence = (diffuse * lightInfluence + ambient);

			const Vec3 outputColor = object_color.GetHadamard(lightColorInfluence).Saturate() * 255.0f;
			return { position, outputColor };
		}

		void SetLightPosition(const Vec3& position) {
			light_position = position;
		}
		void SetDiffuseLightColor(const Vec3& color) {
			diffuse = color;
		}
		void SetLightIntensity(float value) {
			light_intensity = value;
		}
		void SetAmbientLightColor(const Vec3& color) {
			ambient = color;
		}
		void SetObjectColor(const Vec3& color) {
			object_color = color;
		}
	private:
		Vec3 translation;
		Mat3 rotation;
		Vec3 light_position = Vec3{ 0.0f, 0.0f, 2.0f };
		float light_intensity = 2.0f;
		Vec3 diffuse = { 0.8f, 0.7f, 0.5f };
		Vec3 ambient = { 0.1f, 0.1f, 0.2f };
		Vec3 object_color = { 0.8f, 0.8f, 0.8f };

		const float quadratic_attenuation = 2.6f;
		const float linear_attenuation = 1.0f;
		const float constant_attenuation = 0.4f;
	};

	// ----------------------------------------------------------------

	typedef GeometryShaders::DefaultGeometryShader<VertexShader::Output> GeometryShader;
	typedef PixelShaders::PaintByVertexColor<GeometryShader::Output> PixelShader;
public:
	VertexShader vertexShader;
	GeometryShader geometryShader;
	PixelShader pixelShader;
};