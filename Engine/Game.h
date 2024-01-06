/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Game.h																				  *
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
#pragma once

#include "Graphics.h"
#include "PreClip3DSpace.h"
#include "PolygonalMesh.h"
#include "FrameTimer.h"

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	FrameTimer ft;
	PreClip3DSpace pc3ds;
	Cube cube;
	float thetaX = 0.0f;
	float thetaY = 0.0f;
	float thetaZ = 0.0f;
	float offsetZ = 2.0f;
	TexVertex tv0 = { {100.0f,100.0f,100.0f},{0.0f, 0.0f} };
	TexVertex tv1 = { {500.0f,250.0f,100.0f},{1.0f, 0.0f} };
	TexVertex tv2 = { {250.0f,500.0f,100.0f},{1.0f, 1.0f} };
	std::vector<TexVertex> texVs = { tv0, tv1, tv2 };
	IndexedTriangleList<TexVertex> triangle{ texVs,{0,1,2} };
	/********************************/
};