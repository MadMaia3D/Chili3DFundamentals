#pragma once
#include "Matrix3.h"

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
	Output operator()(const Vertex& input) {
		return { input.pos * rotation + translation, input };
	}
private:
	Vec3 translation;
	Mat3 rotation;
};