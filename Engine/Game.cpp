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
	pipeline(gfx),
	itList(Cube::GetColorVertexCube<Vertex>(1.0f))
{
	//itList.vertices[0].color = (Vec3)Colors::Red;
	//itList.vertices[1].color = (Vec3)Colors::Green;
	//itList.vertices[2].color = (Vec3)Colors::Blue;
	//itList.vertices[3].color = (Vec3)Colors::Cyan;
	//itList.vertices[4].color = (Vec3)Colors::White;
	//itList.vertices[5].color = (Vec3)Colors::Magenta;
	//itList.vertices[6].color = (Vec3)Colors::Blue;
	//itList.vertices[7].color = (Vec3)Colors::Black;
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
		offsetZ += 3 * dt;
	}
	if (wnd.kbd.KeyIsPressed('F')) {
		offsetZ = std::max(1.4f, offsetZ - 3 * dt);
	}
	thetaX = wrap_angle(thetaX);
	thetaY = wrap_angle(thetaY);
	thetaZ = wrap_angle(thetaZ);
}

void Game::ComposeFrame() {
	const Mat3 rot = Mat3::RotationX(thetaX) * Mat3::RotationY(thetaY) * Mat3::RotationZ(thetaZ);

	pipeline.BindRotation(rot);
	pipeline.BindTranslation({ 0.0f, 0.0f, offsetZ });
	pipeline.Draw(itList);
}