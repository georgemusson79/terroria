#include "tileWalls.h"

WoodWall::WoodWall(uint16_t X, uint16_t Y, bool placeIntoWorld) : TileWall(WOODWALL,X,Y){
	if (placeIntoWorld) this->create();
}