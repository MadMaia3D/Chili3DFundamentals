#pragma once
#include "Graphics.h"
#include <limits>

class ZBuffer {
public:
	ZBuffer(int width, int height)
		:
		width(width),
		height(height),
		buffer(std::make_unique<float[]>(width * height))
	{}
	void Clear(){
		for (int i = 0; i < width * height; i++) {
			buffer[i] = std::numeric_limits<float>::infinity();
		}
	}
	float GetDepth(int x_position, int y_position) {
		return buffer[y_position * width + x_position];
	}
	void SetDepth(int x_position, int y_position, float depth) {
		buffer[y_position * width + x_position] = depth;
	}
	int GetWidth() const {
		return width;
	}
	int GetHeight() const {
		return height;
	}
	bool TestAndSet(int x_position, int y_position, float depth) {
		if (depth < GetDepth(x_position, y_position)) {
			SetDepth(x_position, y_position, depth);
			return true;
		}
		return false;
	}
private:
	int width;
	int height;
	std::unique_ptr<float[]> buffer;
};