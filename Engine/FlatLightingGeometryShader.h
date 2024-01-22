#pragma once
#include "Triangle.h"
#include "ColorVertex.h"

namespace GeometryShaders {
	template <typename Vertex>
	class FlatLightingGeometryShader {
	public:
		typedef VertexTypes::ColorVertex Output;
	public:
		Triangle<Output> operator()(const Vertex& v0, const Vertex& v1, const Vertex& v2, size_t index) const {
			const Vec3 edge0 = v1.pos - v0.pos;
			const Vec3 edge1 = v2.pos - v0.pos;
			const Vec3 tNormal = edge0.CrossProduct(edge1).Normalize();

			const float lightInfluence = std::max(0.0f, -(light_direction * tNormal));
			const Vec3 lightColorInfluence = (light_diffuse_color * lightInfluence + light_ambient_color);
			const Vec3 outputColor = object_color.GetHadamard(lightColorInfluence).Saturate() * 255.0f;

			return { {v0.pos, outputColor}, {v1.pos, outputColor}, {v2.pos, outputColor} };
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
		Vec3 light_direction = Vec3{ 0.0f, 0.0f, 1.0f };
		Vec3 light_diffuse_color = { 0.8f, 0.7f, 0.5f };
		Vec3 light_ambient_color = { 0.1f, 0.1f, 0.2f };
		Vec3 object_color = { 0.8f, 0.8f, 0.8f };
	};
}