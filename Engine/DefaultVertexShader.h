#pragma once
#include "Matrix.h"

namespace VertexShaders {
	template <class Vertex>
	class DefaultVertexShader {
	public:
		typedef Vertex Output;
	public:
		void BindRotation(const Mat3& rotation_in) {
			rotation = rotation_in;
		}
		void BindTranslation(const Vec3& translation_in) {
			translation = translation_in;
		}
		Output operator()(const Vertex& input) const {
			return { input.pos * rotation + translation, input };
		}
	private:
		Vec3 translation = {0.0f,0.0f,2.0f};
		Mat3 rotation = Mat3::Identity();
	};
}
