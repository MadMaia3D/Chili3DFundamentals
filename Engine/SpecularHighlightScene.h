#pragma once
#include <algorithm>
#include "Scene.h"
#include "Pipeline.h"
#include "SpecularHighlightEffect.h"
#include "FlatColorEffect.h"
#include "Sphere.h"
#include "Meshes.h"

class SpecularHighlightScene : public Scene {
public:
	SpecularHighlightScene(Graphics& gfx)
		:
		pZBuffer(std::make_shared<ZBuffer>(gfx.ScreenWidth, gfx.ScreenHeight)),
		pipeline(gfx, pZBuffer),
		lightPipeline(gfx, pZBuffer),
		itList(Meshes::LoadMeshWithNormals<Vertex>("Models\\teapot_smooth.obj")) {
	}

	void Update(Keyboard& kbd, Mouse& mouse, float deltaTime) override {

		// rotation xy
		if (kbd.KeyIsPressed('W')) {
			thetaX -= deltaTime;
		}
		if (kbd.KeyIsPressed('S')) {
			thetaX += deltaTime;
		}
		if (kbd.KeyIsPressed('A')) {
			thetaY -= deltaTime;
		}
		if (kbd.KeyIsPressed('D')) {
			thetaY += deltaTime;
		}
		// rotation z
		if (kbd.KeyIsPressed('Q')) {
			thetaZ += deltaTime;
		}
		if (kbd.KeyIsPressed('E')) {
			thetaZ -= deltaTime;
		}
		// translation z
		constexpr float zSpeed = 3.0f;
		if (kbd.KeyIsPressed('R')) {
			offsetZ += zSpeed * deltaTime;
		}
		if (kbd.KeyIsPressed('F')) {
			offsetZ = std::max(1.3f, offsetZ - zSpeed * deltaTime);
		}
		thetaX = wrap_angle(thetaX);
		thetaY = wrap_angle(thetaY);
		thetaZ = wrap_angle(thetaZ);

		// lights
		constexpr float lightSpped = 0.25f;
		if (kbd.KeyIsPressed('J')) {
			lightPosition.x -= deltaTime * lightSpped;
		}
		if (kbd.KeyIsPressed('L')) {
			lightPosition.x += deltaTime * lightSpped;
		}
		if (kbd.KeyIsPressed('I')) {
			lightPosition.y += deltaTime * lightSpped;
		}
		if (kbd.KeyIsPressed('K')) {
			lightPosition.y -= deltaTime * lightSpped;
		}
		if (kbd.KeyIsPressed('U')) {
			lightPosition.z -= deltaTime * lightSpped;
		}
		if (kbd.KeyIsPressed('O')) {
			lightPosition.z += deltaTime * lightSpped;
		}

		pipeline.effect.vertexShader.BindTransformation(
			Mat4::RotationX(thetaX) *
			Mat4::RotationY(thetaY) *
			Mat4::RotationZ(thetaZ) *
			Mat4::Translation(0.0f, 0.0f, offsetZ)
		);

		pipeline.effect.pixelShader.SetLightPosition(lightPosition);

		lightPipeline.effect.vertexShader.BindTranslation(lightPosition);
	}

	void Draw() const override {
		const IndexedTriangleList<LightVertex> lightGeo = Sphere::GetPlainSphere<LightVertex>(0.02f);

		pipeline.BeginFrame();
		pipeline.Draw(itList);
		lightPipeline.Draw(lightGeo);
	}

private:
	float thetaX = 0.0f;
	float thetaY = 0.0f;
	float thetaZ = 0.0f;
	float offsetZ = 2.0f;
	Vec3 lightPosition = { 0.0f, 0.0f, 0.5f };

	std::shared_ptr<ZBuffer> pZBuffer;

	typedef Pipeline<SpecularHighlightEffect> MeshPipeline;
	typedef MeshPipeline::Vertex Vertex;
	MeshPipeline pipeline;

	typedef Pipeline<FlatColorEffect> LightPipeline;
	typedef LightPipeline::Vertex LightVertex;
	LightPipeline lightPipeline;

	IndexedTriangleList<Vertex> itList;
};