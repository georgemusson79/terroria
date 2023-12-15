//camera.cpp
#include "game_datatypes.h"
#include "Main.h"
#include "Tiles.h"
#include "Player.h"
#include "Entity.h"
#include "tileWalls.h"
#include <algorithm>
#include "gui.h"
#include "Colliders.h"
bool Camera::setX(double X, bool renderScrOnMove) {
	CornersRect bounds = checkCameraBoundsAt({ X, this->Y });
	if (bounds.bottomLeft.X < 0) X += abs(bounds.bottomLeft.X);
	else if (bounds.bottomRight.X >= Main::WORLD_WIDTH+1) X -= bounds.bottomRight.X - (Main::WORLD_WIDTH+1);
	this->X = X;
	updateCameraBounds();
	if (renderScrOnMove) renderScreen();
	return true;
}

bool Camera::setY(double Y, bool renderScrOnMove) {
	CornersRect bounds = checkCameraBoundsAt({ this->X, Y });
	if (bounds.topLeft.Y < 0) Y += abs(bounds.topLeft.Y);
	else if (bounds.bottomLeft.Y >= Main::WORLD_HEIGHT) Y -= bounds.bottomLeft.Y - Main::WORLD_HEIGHT;
	this->Y = Y;
	updateCameraBounds();
	if (renderScrOnMove) renderScreen();
	return true;
}

bool Camera::setPos(double X, double Y, bool renderScrOnMove) {
	setX(X,false);
	setY(Y,false);
	return true;
}
Camera::Camera(SDL_Renderer* renderer,bool renderScr) {
	this->renderer = renderer;
	setPos(Main::WORLD_WIDTH / 2, Main::WORLD_HEIGHT / 2,renderScr);
}
Camera::Camera(double X, double Y, SDL_Renderer* renderer, bool renderScr) {
	this->renderer = renderer;
	setPos(X, Y,renderScr);
}

Camera::Camera() {
	this->renderer = Main::renderer;
	setPos(0, 0, false);
}
bool Camera::renderTiles() {
	int size = 0;
	std::vector<Vector2> pos = {};
	for (int x = std::floor(cameraBounds.bottomLeft.X); x <= std::ceil(cameraBounds.bottomRight.X)+zoomScale; x++) {
		for (int y = std::floor(cameraBounds.topLeft.Y); y <= std::ceil(cameraBounds.bottomLeft.Y)+zoomScale; y++) {
			if ((x >= 0 && y >= 0) && (x <= Main::WORLD_WIDTH && y <= Main::WORLD_HEIGHT)) {
				if (Main::tiles[x][y] != nullptr) {
					if (Main::tiles[x][y]->isMultiTile) {
						if (std::find(pos.begin(), pos.end(), Vector2(x, y)) == pos.end()) {
							if (Main::tiles[x][y]->draw(renderer, *this)) size++;
							pos.push_back({ x,y });
						}
					}
					else if (Main::tiles[x][y]->draw(renderer, *this)) size++;
				}
			}
		}
	}
	return true;
}

bool Camera::renderTileWalls() {
	int size = 0;
	for (int x = std::floor(cameraBounds.bottomLeft.X); x <= std::ceil(cameraBounds.bottomRight.X) + zoomScale; x++) {
		for (int y = std::floor(cameraBounds.topLeft.Y); y <= std::ceil(cameraBounds.bottomLeft.Y) + zoomScale; y++) {
			if ((x >= 0 && y >= 0) && (x <= Main::WORLD_WIDTH && y <= Main::WORLD_HEIGHT)) {
				if (Main::tileWalls[x][y] != nullptr) {
					if (Main::tileWalls[x][y]->draw(renderer, *this)) size += 1;
				}
			}
		}
	}
	return true;
}

bool Camera::renderEntities() {
	for (Entity* entity : Main::entities) entity->renderEntity();
	return true;
}
bool Camera::renderScreen(bool renderHitboxes) {
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, Main::backgrounds[DEFAULT],NULL,NULL);
	this->renderTileWalls();
	this->renderTiles();
	this->renderEntities();
	if (Main::player != nullptr) {
		this->renderPlayer(Main::player);
		this->renderPlayerGUI(Main::player);
		if (renderHitboxes == true) Main::player->renderHitboxes();
	}

	SDL_RenderPresent(renderer);
	return true;

	if (renderHitboxes) {
		for (Entity* entity : Main::entities) entity->renderHitboxes();
	}
	
}

Vector2 Camera::getPos() {
	return {(double)this->X,(double)this->Y };
}

bool Camera::setZoom(int scale, bool renderScrOnZoom) {
	this->zoomScale = scale;
	return true;
}

int Camera::zoom() {
	return this->zoomScale;
}

void Camera::updateCameraBounds() {
	this->cameraBounds= this->checkCameraBoundsAt({ this->X, this->Y });
}



void Camera::blockPlacementPositionDraw(Vector2 pos) {
	//wip do not use
	CornersRect bounds = this->cameraBounds;
	Vector2 topLeft = bounds.topLeft;
	int zoom = this->zoom();
	double newX = (std::round(pos.X) - topLeft.X) * zoom;
	double newY = (std::round(pos.Y) - topLeft.Y) * zoom;
	newX = std::round(newX);
	newY = std::round(newY);
	SDL_Rect dstrect = { newX,newY,this->zoom(),this->zoom() };
	if ((dstrect.x < Main::WINDOW_WIDTH && dstrect.x + dstrect.w>0) && (dstrect.y < Main::WINDOW_HEIGHT && dstrect.y + dstrect.h>0)) {
		SDL_SetRenderDrawColor(this->renderer, 255, 255, 0, 120);
		SDL_RenderFillRect(this->renderer, &dstrect);
		SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
	}
}




void Camera::renderPlayer(Player* player) {
	player->renderEntity();
}

void Camera::renderPlayerGUI(Player* player) {
	GUIinterface::renderPlayerHealh(Main::player);
	GUIinterface::renderPlayerMana(Main::player);
	if (Main::player->inventoryOpen) {
		GUIinterface::renderPlayerInventory(Main::player);
		GUIinterface::renderPlayerInvArmor(Main::player);
	}
	else GUIinterface::renderPlayerHotbar(Main::player);
}


CornersRect Camera::checkCameraBoundsAt(Vector2 pos) {
	CornersRect bounds;
	double lowerY = pos.Y + (Main::convertPixelSizeToTileSize(Main::WINDOW_HEIGHT,this) / 2);
	double upperY = pos.Y - (Main::convertPixelSizeToTileSize(Main::WINDOW_HEIGHT,this) / 2);
	double upperX = pos.X + (Main::convertPixelSizeToTileSize(Main::WINDOW_WIDTH,this) / 2);
	double lowerX = pos.X - (Main::convertPixelSizeToTileSize(Main::WINDOW_WIDTH,this) / 2);

	bounds.bottomLeft = { lowerX,lowerY };
	bounds.bottomRight = { upperX,lowerY };
	bounds.topLeft = { lowerX,upperY };
	bounds.topRight = { upperX,upperY };
	return bounds;
}