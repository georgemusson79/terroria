//colliders.h
#pragma once
#include "game_datatypes.h"
#include "Line.h"
#include <array>
#include <cmath>
class Tile;

enum class hitboxType {
    SQUARE, CIRCLE,ROTATABLE
};




class Hitbox {
public:
    bool active = true;
    hitboxType type;
    Vector2 position = { 0,0 };
    virtual ~Hitbox();
    virtual Vector2 getCenter() = 0;
    virtual void setCenter(Vector2 position) = 0;
    virtual bool collidesWith(Vector2 pos)=0;
    virtual bool collidesWith(Tile* tile) = 0;
    virtual bool collidesWith(Hitbox* inputHitbox) = 0;
    virtual void renderHitbox() = 0;
    virtual void setPosition(Vector2 position);
    virtual std::vector<Tile*> collidesWithTiles() = 0;
    Hitbox(hitboxType type);
    static std::array<Line, 4> generateLines(std::array<Vector2, 4> corners) {
        std::array<Line, 4> lines = {};
        for (int i = 0; i < corners.size(); i++) {
            Vector2 first = corners[i];
            Vector2 next = (i == corners.size() - 1) ? corners[0] : corners[i + 1];
            Line line = { first,next };
            lines[i] = line;
        }
        return lines;

    }
};


class SquareHitbox : public Hitbox {
public:
    hitboxType type = hitboxType::SQUARE;
    float h = 0;
    float w = 0;
    SquareHitbox(Vector2 position, float width, float height);
    SquareHitbox();
    void setCenter(Vector2 position) override;
    std::array<Vector2, 4> getCorners();
    std::array<Line, 4> getLines();
    bool collidesWith(Vector2 pos) override;
    Vector2 getCenter() override;
    bool collidesWith(Tile* tile) override;
    bool collidesWith(Hitbox* inputHitbox) override;
    std::vector<Tile*> collidesWithTiles()  override;
    void renderHitbox() override;
};

class CircleHitbox :public Hitbox {
public:
    hitboxType type = hitboxType::CIRCLE;
    double r = 0;
    CircleHitbox(Vector2 position, float radius);
    CircleHitbox();
    void setCenter(Vector2 position) override;
    Vector2 getCenter() override;
    bool collidesWith(Vector2 pos) override;
    bool collidesWith(Tile* tile) override;
    bool collidesWith(Hitbox* inputHitbox) override;
    void renderHitbox() override;
    std::vector<Tile*> collidesWithTiles()  override;

};


class RotatableHitbox : public Hitbox{
private:
    std::array<Line, 4> generateLines();
    std::vector<Line> drawLine = {};
    double rotation = 0;
public:
    void setRotation(double rotation);
    double getRotation();
    double width = 0;
    double height = 0;
    std::array<Vector2, 4> corners = {};
    RotatableHitbox(Vector2 center, double w, double h);
    void setPosition(Vector2 position) override;
    void setCenter(Vector2 position) override;

    void updateCorners();
    Vector2 getCenter() override;
    bool collidesWith(Line line);
    bool collidesWithSq(SquareHitbox* sq);
    bool collidesWithRotatable(RotatableHitbox* sq);
    Vector2 getClosestCorner(Vector2 pos);
    void renderHitbox() override;
    bool collidesWith(Vector2 pos) override;
    bool collidesWith(Tile* tile) override;
    bool collidesWith(Hitbox* inputHitbox) override;
    std::vector<Tile*> collidesWithTiles()  override;

};