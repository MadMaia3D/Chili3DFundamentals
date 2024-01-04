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
		offsetZ += dt;
	}
	if (wnd.kbd.KeyIsPressed('F')) {
		offsetZ = std::max(1.4f, offsetZ - dt);
	}
	thetaX = wrap_angle(thetaX);
	thetaY = wrap_angle(thetaY);
	thetaZ = wrap_angle(thetaZ);
}

void Game::ComposeFrame() {
	constexpr Color colorList[] = {
		{255,0,0} ,{255,64,0},
		{255,128,0} ,{255,196,0},
		{255,255,0}, {196,255,0},
		{128,255,0}, {64,255,0},
		{0,255,0}, {0,255,64},
		{0,255,128}, {0,255,196}
	};


	IndexedTriangleList tris = cube.GetTriangles();

	const Mat3 rot = Mat3::RotationX(thetaX) * Mat3::RotationY(thetaY) * Mat3::RotationZ(thetaZ);
	for (auto& v : tris.vertices) {
		v *= rot;
		v += {0.0f, 0.0f, offsetZ};
		pc3ds.Transform(v);
	}

	for (auto i = tris.indices.cbegin(), end = tris.indices.cend(); i != end; std::advance(i, 3)) {
		const int colorIndex = (int)std::distance(tris.indices.cbegin(), i) / 3;
		gfx.DrawTriangle(tris.vertices[*i], tris.vertices[*std::next(i)], tris.vertices[*std::next(i, 2)], colorList[colorIndex]);
	}

}