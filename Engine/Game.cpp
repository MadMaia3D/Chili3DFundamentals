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
#include "Matrix3.h"
#include "ChiliMath.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	cube(1.0f) {
}

void Game::Go() {
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel() {
	const float dt = ft.Mark();
	if (wnd.kbd.KeyIsPressed('W')) {
		thetaX += dt;
	}
	if (wnd.kbd.KeyIsPressed('S')) {
		thetaX -= dt;
	}
	if (wnd.kbd.KeyIsPressed('A')) {
		thetaY += dt;
	}
	if (wnd.kbd.KeyIsPressed('D')) {
		thetaY -= dt;
	}
	if (wnd.kbd.KeyIsPressed('Q')) {
		thetaZ += dt;
	}
	if (wnd.kbd.KeyIsPressed('E')) {
		thetaZ -= dt;
	}
	if (wnd.kbd.KeyIsPressed('R')) {
		offsetZ += 3 * dt;
	}
	if (wnd.kbd.KeyIsPressed('F')) {
		offsetZ = std::max(1.4f, offsetZ - 3 * dt);
	}
	thetaX = wrap_angle(thetaX);
	thetaY = wrap_angle(thetaY);
	thetaZ = wrap_angle(thetaZ);

	if (wnd.mouse.LeftIsPressed()) {
		tv1.pos.x = (float)wnd.mouse.GetPosX();
		tv1.pos.y = (float)wnd.mouse.GetPosY();
	}
	if (wnd.mouse.RightIsPressed()) {
		tv0.pos.x = (float)wnd.mouse.GetPosX();
		tv0.pos.y = (float)wnd.mouse.GetPosY();
	}
}

void Game::ComposeFrame() {
	constexpr Color colorList[] = {
		{255,0,0} ,{255,128,0},
		{255,255,0} ,{128,255,0},
		{0,255,0}, {0,255,128},
		{0,255,255}, {0,128,255},
		{0,0,255}, {128,0,255},
		{255,0,255}, {255,0,128}
	};

	IndexedTriangleList<TexVertex> tris = cube.GetTrianglesTex();

	const Mat3 rot = Mat3::RotationX(thetaX) * Mat3::RotationY(thetaY) * Mat3::RotationZ(thetaZ);
	for (auto& v : tris.vertices) {
		v.pos *= rot;
		v.pos += {0.0f, 0.0f, offsetZ};
	}

	for (int i = 0; i < tris.cullFlags.size(); i++) {
		const Vec3& v0 = tris.vertices[tris.indices[i * 3]].pos;
		const Vec3& v1 = tris.vertices[tris.indices[i * 3 + 1]].pos;
		const Vec3& v2 = tris.vertices[tris.indices[i * 3 + 2]].pos;
		tris.cullFlags[i] = (v1 - v0).CrossProduct(v2 - v0) * v0 <= 0.0f;
	}

	for (auto& v : tris.vertices) {
		pc3ds.Transform(v.pos);
	}

	for (int i = 0; i < tris.cullFlags.size(); i++) {
		const TexVertex& v0 = tris.vertices[tris.indices[i * 3]];
		const TexVertex& v1 = tris.vertices[tris.indices[i * 3 + 1]];
		const TexVertex& v2 = tris.vertices[tris.indices[i * 3 + 2]];
		if (tris.cullFlags[i]) {
			gfx.DrawTriangle(v0, v1, v2, Colors::Gray);
		}
	}

	//gfx.DrawTriangle(tv0, tv1, tv2, Colors::White);
}