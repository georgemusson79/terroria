#pragma once
#include "tileWall.h"

enum WallIDs {
	WOODWALL
};

class WoodWall : public TileWall {
public:
	WoodWall(uint16_t X, uint16_t Y, bool placeIntoWorld = true);
};