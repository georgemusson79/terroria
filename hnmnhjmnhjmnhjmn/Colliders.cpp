//colliders.cpp
#include "Main.h"
#include "Tile.h"
#include "Colliders.h"
#include "Line.h"

Hitbox::~Hitbox() {
}
Hitbox::Hitbox(hitboxType type) : type(type){

}

void Hitbox::setPosition(Vector2 position) {
	this->position = position;
}


CircleHitbox::CircleHitbox(Vector2 position, float radius) : Hitbox::Hitbox(hitboxType::CIRCLE) {
	this->position = position;
	this->r = radius;
}

CircleHitbox::CircleHitbox() : Hitbox::Hitbox(hitboxType::CIRCLE) {
	this->r = 0;
}

bool CircleHitbox::collidesWith(Vector2 pos) {
	if (pos.distance(this->position) < this->r && this->active) return true;
	return false;
}

Vector2 CircleHitbox::getCenter() {
	return this->position;
 }

void CircleHitbox::setCenter(Vector2 position) {
	this->position = position;
}

bool CircleHitbox::collidesWith(Tile* tile) {
	if (tile != nullptr && this->active) {
		Vector2 tilepos = { tile->X,tile->Y };
		SquareHitbox tilehbox = tile->getHitbox();
		return tilehbox.collidesWith(this);
	}
	return false;
}

bool CircleHitbox::collidesWith(Hitbox* hitbox) {
	if (this->active) {
		switch (hitbox->type) {
		case (hitboxType::CIRCLE): 
		{
			CircleHitbox* cbox = dynamic_cast<CircleHitbox*>(hitbox);
			return this->position.distance(cbox->position) < this->r + cbox->r;
		}

		case (hitboxType::SQUARE):
			return hitbox->collidesWith(this);
		case (hitboxType::ROTATABLE):
			return dynamic_cast<RotatableHitbox*>(hitbox)->collidesWith(this);
		}
	}
	return false;
}

void::CircleHitbox::renderHitbox() {
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(Main::renderer, &r, &g, &b, &a);
	std::vector<SDL_Point> points = {};
	iVector2 pixelCenter = Main::convertWorldPosToCameraPos(this->position);
	int pixelRadius = Main::convertTileSizeToPixelSize(this->r);
	int prevY = pixelRadius;
	int y;
	(this->active) ? SDL_SetRenderDrawColor(Main::renderer, 0, 0, 0, 255) : SDL_SetRenderDrawColor(Main::renderer, 255, 0, 0, 255);
	for (int x = 0; x <= pixelRadius; x++) {
		y = sqrt((pixelRadius * pixelRadius) - (x * x));
		for (int dy = y; dy <= prevY; dy++) {
			points.push_back({ pixelCenter.X + x,pixelCenter.Y + dy });
			points.push_back({ pixelCenter.X - x,pixelCenter.Y + dy });
			points.push_back({ pixelCenter.X + x,pixelCenter.Y - dy });
			points.push_back({ pixelCenter.X - x,pixelCenter.Y - dy });
		}
		prevY = y;
	}
	SDL_RenderDrawPoints(Main::renderer, &points[0], points.size()-1);
	SDL_SetRenderDrawColor(Main::renderer, r, g, b, a);
}

void SquareHitbox::renderHitbox() {
	Uint8 r, g, b,a;
	SDL_GetRenderDrawColor(Main::renderer, &r, &g, &b, &a);
	iVector2 dim = { Main::convertTileSizeToPixelSize(this->w),Main::convertTileSizeToPixelSize(this->h) };
	iVector2 pos = Main::convertWorldPosToCameraPos(this->position);
	SDL_Rect rect = { pos.X,pos.Y,dim.X,dim.Y };
	(this->active) ? SDL_SetRenderDrawColor(Main::renderer, 0, 0, 0, 255) : SDL_SetRenderDrawColor(Main::renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(Main::renderer, &rect);
	SDL_SetRenderDrawColor(Main::renderer, r, g, b, a);
}

SquareHitbox::SquareHitbox(Vector2 position, float width, float height) : Hitbox::Hitbox(hitboxType::SQUARE) {
	this->position = position;
	this->w = width;
	this->h = height;
}

Vector2 SquareHitbox::getCenter() {
	double x = this->position.X + (this->w / 2);
	double y = this->position.Y + (this->h / 2);
	return { x,y };
}

void SquareHitbox::setCenter(Vector2 position) {
	this->position.X = position.X - (this->w / 2);
	this->position.Y= position.Y - (this->h / 2);
}

SquareHitbox::SquareHitbox() : Hitbox::Hitbox(hitboxType::SQUARE) {
	this->w = 0;
	this->h = 0;
}

bool SquareHitbox::collidesWith(Vector2 pos) {
	if (this->active) {
		if (pos.X >= this->position.X && pos.Y >= this->position.Y) {
			if (pos.X <= this->position.X + this->w && pos.Y <= this->position.Y + this->h) return true;
		}
	}
	return false;
}

bool SquareHitbox::collidesWith(Hitbox* inputHitbox) {
	if (this->active) {
		switch (inputHitbox->type) {

		case (hitboxType::ROTATABLE):
			return dynamic_cast<RotatableHitbox*>(inputHitbox)->collidesWith(this);

		case (hitboxType::SQUARE):
		{
			SquareHitbox* sbox = dynamic_cast<SquareHitbox*>(inputHitbox);
			double halfsumXside = (this->w + sbox->w) / 2;
			double halfsumYside = (this->h + sbox->h) / 2;
			Vector2 centerA = { this->position.X + (this->w / 2),this->position.Y + (this->h / 2) };
			Vector2 centerB = { sbox->position.X + (sbox->w / 2),sbox->position.Y + (sbox->h / 2) };
			if (abs(centerA.X - centerB.X) < halfsumXside && abs(centerA.Y - centerB.Y) < halfsumYside) return true;
		}
			break;

		case (hitboxType::CIRCLE):
		{
			Vector2 comparePos;
			Vector2 squareCenter = { this->position.X + (this->w / 2),this->position.Y + (this->h / 2) };
			CircleHitbox* cbox = dynamic_cast<CircleHitbox*>(inputHitbox);
			//check if inside square
			if (cbox->position.X > this->position.X && cbox->position.X<this->position.X + this->w &&
				cbox->position.Y>this->position.Y && cbox->position.Y < this->position.Y + this->h) return true;
			if (cbox->position.X <= squareCenter.X) comparePos.X = this->position.X;
			//check for outside square
			else if (cbox->position.X > squareCenter.X) comparePos.X = this->position.X + this->w;

			if (cbox->position.Y <= squareCenter.Y) comparePos.Y = this->position.Y;
			else if (cbox->position.Y > squareCenter.Y) comparePos.Y = this->position.Y + this->h;

			if (cbox->position.distance(comparePos) < cbox->r) return true;
		}
			break;
		}
	}
	return false;
}

bool SquareHitbox::collidesWith(Tile* tile) {
	if (this->active) {
		if (tile != nullptr) {
			SquareHitbox tilehitbox = tile->getHitbox();
			return this->collidesWith(&tilehitbox);
		}
	}
	return false;
}

std::array<Vector2,4> SquareHitbox::getCorners() {
	std::array<Vector2, 4> corners;
	corners[0] = this->position;
	corners[1] = { this->position.X + this->w,this->position.Y };
	corners[2] = { corners[1].X,corners[1].Y + this->h };
	corners[3] = { position.X,position.Y + this->h };
	return corners;
}

std::array<Line, 4> SquareHitbox::getLines() {
	return Hitbox::generateLines(this->getCorners());
}

std::vector<Tile*> SquareHitbox::collidesWithTiles() {
	std::vector <Tile*> tile = {};
	if (this->active) {
		for (int x = this->position.X; x < this->position.X + this->w; x++) {
			for (int y = this->position.Y; y < this->position.Y + this->h; y++) {
				if (Main::tiles[x][y] != nullptr && Main::tiles[x][y]->isSolid) if (this->collidesWith(Main::tiles[x][y])) tile.push_back(Main::tiles[x][y]);
			}
		}
	}
	return tile;
}

std::vector<Tile*> CircleHitbox::collidesWithTiles() {
	std::vector <Tile*> tile = {};
	if (this->active) {
		Vector2 start = { position.X - r,position.Y - r };
		Vector2 end = { position.X + r,position.Y + r };
		for (int x = start.X; x <= end.X; x++) {
			for (int y = start.Y; y <= end.Y; y++) {
				if (Main::tiles[x][y] != nullptr && Main::tiles[x][y]->isSolid) if (this->collidesWith(Main::tiles[x][y])) tile.push_back(Main::tiles[x][y]);
			}
		}
	}
	return tile;
}


RotatableHitbox::RotatableHitbox(Vector2 center, double w, double h) : Hitbox(hitboxType::ROTATABLE) {
	this->position = center;
	this->width = w;
	this->height = h;
	this->updateCorners();
}

void RotatableHitbox::setRotation(double rotation) {
	this->rotation = rotation;
	this->updateCorners();
}
double RotatableHitbox::getRotation() {
	return rotation;
}

bool RotatableHitbox::collidesWithRotatable(RotatableHitbox* sq) {
	for (int i = 0; i < corners.size(); i++) {
		Vector2 first = corners[i];
		Vector2 next = (i == corners.size() - 1) ? corners[0] : corners[i + 1];
		Line l1 = { first,next };
		for (int i = 0; i < corners.size(); i++) {
			Vector2 first = sq->corners[i];
			Vector2 next = (i == corners.size() - 1) ? sq->corners[0] : sq->corners[i + 1];
			Line l2 = { first,next };
			if (l1.checkIfIntersects(l2)) return true;
		}
	}
	return false;
}

void RotatableHitbox::updateCorners() {
	Vector2 c1 = { position.X - (width / 2),position.Y - (height / 2) };
	Vector2 c2 = { c1.X + width,c1.Y };
	Vector2 c3 = { c2.X,c2.Y + height };
	Vector2 c4 = { c1.X,c1.Y + height };
	corners[0] = c1;
	corners[1] = c2;
	corners[2] = c3;
	corners[3] = c4;
	for (int i = 0; i < corners.size(); i++) {
		corners[i] = Main::rotatePt(corners[i], position, rotation);
	}
}

void RotatableHitbox::setCenter(Vector2 position) {
	this->position = position;
}

void RotatableHitbox::renderHitbox() {
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(Main::renderer, &r, &g, &b, &a);
	std::array<Vector2, 4>& cs = corners;
	if (this->active) {
		SDL_SetRenderDrawColor(Main::renderer, 0, 0, 0, 255);
	}
	for (int i = 0; i < corners.size(); i++) {
		iVector2 first = Main::convertWorldPosToCameraPos(cs[i]);
		Vector2 temp = (i == cs.size() - 1) ? cs[0] : cs[i + 1];
		iVector2 next = Main::convertWorldPosToCameraPos(temp);
		SDL_RenderDrawLine(Main::renderer, first.X, first.Y, next.X, next.Y);
	}
	for (auto& line : drawLine) line.draw();
	drawLine = {};
	SDL_SetRenderDrawColor(Main::renderer, r, g, b, a);
}

bool RotatableHitbox::collidesWith(Line line) {
	for (int i = 0; i < corners.size(); i++) {
		Vector2 first = corners[i];
		Vector2 next = (i == corners.size() - 1) ? corners[0] : corners[i + 1];
		Line l2 = { first,next };
		if (l2.checkIfIntersects(line)) return true;
	}
	return false;
}

Vector2 RotatableHitbox::getCenter() {
	return this->position;
}

bool RotatableHitbox::collidesWithSq(SquareHitbox* sq) {
	for (Line& l1 : Hitbox::generateLines(sq->getCorners())) {
		for (Line& l2 : Hitbox::generateLines(this->corners)) {
			if (l1.checkIfIntersects(l2)) {
				return true;
			}
		}
	}

	for (Vector2 corner : this->corners) {
		if (sq->collidesWith(corner)) return true;
	}
	double rotation = this->getRotation();
	this->setRotation(0);
	Vector2 topLeft = this->corners[0];
	Vector2 bottmRight = this->corners[2];
	this->setRotation(rotation);
	for (Vector2 corner : sq->getCorners()) {
		Vector2 newCorner = Main::rotatePt(corner, this->position, -rotation);
		if (newCorner.X<bottmRight.X && newCorner.X>topLeft.X && newCorner.Y<bottmRight.Y && newCorner.Y>topLeft.Y) return true;
	}
	return false;
}


bool RotatableHitbox::collidesWith(Vector2 pos) {
	for (auto line : generateLines()) {
		if (line.isOnLine(pos)) return true;
	}
	double rotation = this->getRotation();
	this->setRotation(0);
	Vector2 topLeft = corners[0];
	Vector2 bottomRight = corners[2];
	this->setRotation(rotation);
	if (pos.X > topLeft.X && pos.X < bottomRight.X && pos.Y<bottomRight.Y && pos.Y>topLeft.Y) return true;
	return false;
}
bool RotatableHitbox::collidesWith(Tile* tile) {
	return true;
}
bool RotatableHitbox::collidesWith(Hitbox* inputHitbox) {
	switch (inputHitbox->type) {
	case (hitboxType::ROTATABLE):
		return this->collidesWithRotatable(dynamic_cast<RotatableHitbox*>(inputHitbox));
		break;
	case (hitboxType::SQUARE):
		return this->collidesWithSq(dynamic_cast<SquareHitbox*>(inputHitbox));
		break;
	case (hitboxType::CIRCLE):
		CircleHitbox* cbox = dynamic_cast<CircleHitbox*>(inputHitbox);
		//rotate both circle and square so square is parallel to the x axis
		double rotation = this->getRotation();
		Vector2 circlePos = Main::rotatePt(cbox->position, this->position, -rotation);
		this->setRotation(0);

		//check if circle is inside square
		Vector2& topLeft = corners[0];
		Vector2& bottomRight = corners[2];
		if (circlePos.X<bottomRight.X && circlePos.X>topLeft.X && circlePos.Y<bottomRight.Y && circlePos.Y>topLeft.Y) {
			this->setRotation(rotation);
			return true;
		}
		
		//get side circle is on
		double x = (circlePos.X < this->position.X) ? this->position.X - (this->width / 2) : this->position.X + (this->width / 2);
		double y = (circlePos.Y < this->position.Y) ? this->position.Y - (this->height / 2) : this->position.Y + (this->height / 2);
		//get distance between edge and radius
		if (circlePos.distance({ x,y }) < cbox->r) {
			this->setRotation(rotation);
			return true;
		}
		this->setRotation(rotation);
		break;
	}
	return false;
}

std::array<Line, 4> RotatableHitbox::generateLines() {
	std::array<Line, 4> lines = {};
	for (int i = 0; i < corners.size(); i++) {
		Vector2 first = corners[i];
		Vector2 next = (i == corners.size() - 1) ? corners[0] : corners[i + 1];
		Line line= { first,next };
		lines[i] = line;
	}
	return lines;

}

Vector2 RotatableHitbox::getClosestCorner(Vector2 pos) {
	Vector2 closest;
	double nearestDist = std::numeric_limits<double>::max();
	for (int i = 0; i < corners.size(); i++) {
		double distance = corners[i].distance(pos);
		if (distance < nearestDist) {
			closest = corners[i];
			nearestDist = distance;
		}
	}
	return closest;
}

std::vector<Tile*> RotatableHitbox::collidesWithTiles() {
	return {};
}


void RotatableHitbox::setPosition(Vector2 position) {
	this->position = position;
	this->updateCorners();
}