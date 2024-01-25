#pragma once
#include "Mouse.h"
#include "Keyboard.h"
#include "Graphics.h"

class Scene {
public:
	virtual void Update(Keyboard& kbd, Mouse& mouse, float deltaTime) = 0;
	virtual void Draw() const = 0;
	virtual ~Scene() = default;
};