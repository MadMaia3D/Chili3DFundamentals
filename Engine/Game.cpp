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
	/*const float dt = ft.Mark();
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
	thetaZ = wrap_angle(thetaZ);*/

	constexpr float speed = 1.0f;
	if (wnd.mouse.LeftIsPressed()) {
		v1 = (Vec2)wnd.mouse.GetPos();
	}
	if (wnd.mouse.RightIsPressed()) {
		v2 = (Vec2)wnd.mouse.GetPos();
	}
	if (wnd.kbd.KeyIsPressed('W')) {
		v0.y -= speed;
	}
	if (wnd.kbd.KeyIsPressed('S')) {
		v0.y += speed;
	}
	if (wnd.kbd.KeyIsPressed('A')) {
		v0.x -= speed;
	}
	if (wnd.kbd.KeyIsPressed('D')) {
		v0.x += speed;
	}
}

void Game::ComposeFrame() {
	/*IndexedLineList lines = cube.GetLines();
	const Mat3 rot = Mat3::RotationX(thetaX) * Mat3::RotationY(thetaY) * Mat3::RotationZ(thetaZ);
	for (auto& v : lines.vertices) {
		v *= rot;
		v += {0.0f, 0.0f, offsetZ};
		pc3ds.Transform(v);
	}

	for (auto i = lines.indices.cbegin(), end = lines.indices.cend(); i != end; std::advance(i, 2)) {
		gfx.DrawLine(lines.vertices[*i], lines.vertices[*std::next(i)], Colors::White);
	}*/

	/*gfx.DrawTriangle({ 100.0f ,100.0f }, { 300.0f ,100.0f }, { 200.0f , 200.0f }, Colors::White);
	gfx.DrawTriangle({ 100.0f ,400.0f }, { 300.0f ,400.0f }, { 200.0f , 300.0f }, Colors::White);
	gfx.DrawTriangle({ 400.0f ,200.0f }, { 300.0f ,300.0f }, { 500.0f , 500.0f }, Colors::White);*/
	gfx.DrawTriangle(v0, v1, v2, Colors::White);
}