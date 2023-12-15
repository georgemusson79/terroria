#include <iostream>
#include "Vector2.h"
#pragma once
//game_datatypes.h
struct SDL_Texture;


struct CornersRect {
	Vector2 bottomLeft;
	Vector2 topLeft;
	Vector2 bottomRight;
	Vector2 topRight;

	void getbounds() {
		std::cout << "bottomX:" << bottomLeft.X << "bottomY:" << bottomLeft.Y << "topX:" << topRight.X << "topY:" << topRight.Y <<"\n";
	}
};

struct TextureInfo {
	SDL_Texture* texture = nullptr;
	int textureWidth;
	int textureHeight;
	~TextureInfo();
};

