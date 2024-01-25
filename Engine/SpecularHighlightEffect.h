#pragma once
#include "NormalsVertex.h"
#include "DefaultGeometryShader.h"
#include "Colors.h"
#include <algorithm>

class SpecularHighlightEffect {
public:
	typedef VertexTypes::NormalsVertex Vertex;

	// --------------------------- VERTEX SHADER -------------------------------------
	class VertexShader {
	public:
		class Output {
		public:
			Output() = default;
			Output(const Vec3& position)
				:
				pos(position) {
			}
			Output(const Vec3& position, const Vec3& normal, const Vec3& worldPosition)
				:
				pos(position),
				normal(normal),
				worldPos(worldPosition) {
			}
			Output(const Vec3& position, const Output& v)
				:
				pos(position),
				normal(v.normal),
				worldPos(v.worldPos) {
			}
			Output& operator+=(const Output& rhs) {
				pos += rhs.pos;
				normal += rhs.normal;
				worldPos += rhs.worldPos;
				return *this;
			}
			Output operator+(const Output& rhs) const {
				return Output(*this) += rhs;
			}
			Output& operator-=(const Output& rhs) {
				pos -= rhs.pos;
				normal -= rhs.normal;
				worldPos -= rhs.worldPos;
				return *this;
			}
			Output operator-(const Output& rhs) const {
				return Output(*this) -= rhs;
			}
			Output& operator*=(float rhs) {
				pos *= rhs;
				normal *= rhs;
				worldPos *= rhs;
				return *this;
			}
			Output operator*(float rhs) const {
				return Output(*this) *= rhs;
			}
			Output& operator/=(float rhs) {
				pos /= rhs;
				normal /= rhs;
				worldPos /= rhs;
				return *this;
			}
			Output operator/(float rhs) const {
				return Output(*this) /= rhs;
			}
			Vec3 pos{};
			Vec3 normal{};
			Vec3 worldPos{};
		};
	public:
		void BindRotation(const Mat3& rotation_in) {
			rotation = rotation_in;
		}
		void BindTranslation(const Vec3& translation_in) {
			translation = translation_in;
		}
		Output operator()(const Vertex& input) const {
			const Vec3 worldPosition = input.pos * rotation + translation;
			return { worldPosition, input.normal * rotation, worldPosition };
		}
	private:
		Vec3 translation = { 0.0f,0.0f,2.0f };
		Mat3 rotation = Mat3::Identity();
	};
	// --------------------------- GEOMETRY SHADER -------------------------------------

	typedef GeometryShaders::DefaultGeometryShader<VertexShader::Output> GeometryShader;

	// --------------------------- PIXEL SHADER -------------------------------------
	class PixelShader {
	public:
		Color operator()(const GeometryShader::Output& scanPosInfo) const {
			// vertex to light data
			const Vec3 v_to_l = light_position - scanPosInfo.worldPos;
			const float dist = v_to_l.Lenght();
			const Vec3 dir = v_to_l / dist;
			// re-normalize interpolated surface normal
			const Vec3 surf_normal = scanPosInfo.normal.GetNormalized();
			// calculate reflected light vector
			const Vec3 lightNormalProjection = surf_normal * (v_to_l * surf_normal);
			const Vec3 reflectedLightVector = (lightNormalProjection * 2) - v_to_l;
			// calculate specular intensity based on angle between reflected light vector and view vector
			const float highlightDotProduct = reflectedLightVector.GetNormalized() * -scanPosInfo.worldPos.GetNormalized();
			const float highlightFactor = std::pow(std::max(0.0f, highlightDotProduct), specular_power);
			const float highlightInfluence = highlightFactor * specular_intensity;
			// calculate attenuation
			const float attenuation = 1.0f /
				(quadratic_attenuation * sq(dist) + linear_attenuation * dist + constant_attenuation);
			// calculate intensity based on angle of incidence and attenuation
			const float angleIncidence = std::max(0.0f, (dir * surf_normal));

			const float lightInfluence = angleIncidence * light_intensity * attenuation + highlightInfluence;

			const Vec3 lightColorInfluence = (diffuse * lightInfluence + ambient);

			const Vec3 outputColor = object_color.GetHadamard(lightColorInfluence).Saturate() * 255.0f;
			return Color(outputColor);
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
		Vec3 light_position = Vec3{ 0.0f, 0.0f, 2.0f };
		float light_intensity = 2.0f;
		Vec3 diffuse = { 0.8f, 0.7f, 0.5f };
		int specular_power = 10;
		float specular_intensity = 1.0f;
		Vec3 ambient = { 0.1f, 0.1f, 0.2f };
		Vec3 object_color = { 0.8f, 0.8f, 0.8f };

		const float quadratic_attenuation = 2.6f;
		const float linear_attenuation = 1.0f;
		const float constant_attenuation = 0.4f;
	};
public:
	VertexShader vertexShader;
	GeometryShader geometryShader;
	PixelShader pixelShader;
};