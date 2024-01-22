/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Game.cpp																			  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "Meshes.h"
#include "Sphere.h"
#include "Plane.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	pZBuffer(std::make_shared<ZBuffer>(gfx.ScreenWidth, gfx.ScreenHeight)),
	pipeline(gfx, pZBuffer),
	lightPipeline(gfx, pZBuffer),
	//itList(Meshes::LoadMeshWithNormals<Vertex>("Models\\suzanne_smooth.obj"))
	itList(Plane::GetNormalsPlane<Vertex>(1.2f, 8))
{
}

void Game::Go() {
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel() {
	const float dt = ft.Mark();

	// model
	if (wnd.kbd.KeyIsPressed('W')) {
		thetaX -= dt;
	}
	if (wnd.kbd.KeyIsPressed('S')) {
		thetaX += dt;
	}
	if (wnd.kbd.KeyIsPressed('A')) {
		thetaY -= dt;
	}
	if (wnd.kbd.KeyIsPressed('D')) {
		thetaY += dt;
	}
	if (wnd.kbd.KeyIsPressed('Q')) {
		thetaZ -= dt;
	}
	if (wnd.kbd.KeyIsPressed('E')) {
		thetaZ += dt;
	}
	if (wnd.kbd.KeyIsPressed('R')) {
		offsetZ += 3.0f * dt;
	}
	if (wnd.kbd.KeyIsPressed('F')) {
		offsetZ = std::max(1.3f, offsetZ - 3.0f * dt);
	}
	thetaX = wrap_angle(thetaX);
	thetaY = wrap_angle(thetaY);
	thetaZ = wrap_angle(thetaZ);

	// lights
	constexpr float lightSpped = 0.25f;
	if (wnd.kbd.KeyIsPressed('J')) {
		lightPosition.x -= dt * lightSpped;
	}
	if (wnd.kbd.KeyIsPressed('L')) {
		lightPosition.x += dt * lightSpped;
	}
	if (wnd.kbd.KeyIsPressed('I')) {
		lightPosition.y += dt * lightSpped;
	}
	if (wnd.kbd.KeyIsPressed('K')) {
		lightPosition.y -= dt * lightSpped;
	}
	if (wnd.kbd.KeyIsPressed('U')) {
		lightPosition.z -= dt * lightSpped;
	}
	if (wnd.kbd.KeyIsPressed('O')) {
		lightPosition.z += dt * lightSpped;
	}

	const Mat3 rot = Mat3::RotationX(thetaX) * Mat3::RotationY(thetaY) * Mat3::RotationZ(thetaZ);
	pipeline.effect.vertexShader.BindRotation(rot);
	pipeline.effect.vertexShader.BindTranslation({ 0.0f, 0.0f, offsetZ });
	pipeline.effect.vertexShader.SetLightPosition(lightPosition);
}

void Game::ComposeFrame() {

	const IndexedTriangleList<LightVertex> lightGeo = Sphere::GetPlainSphere<LightVertex>(0.02f);
	lightPipeline.effect.vertexShader.BindTranslation(lightPosition);

	pipeline.BeginFrame();
	pipeline.Draw(itList);

	lightPipeline.Draw(lightGeo);
}