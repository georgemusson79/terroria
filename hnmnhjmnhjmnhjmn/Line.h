#pragma once
#include "Main.h"

struct Line {
    Vector2 start;
    Vector2 end;
    Line() {
        start = { 0,0 };
        end = { 0,0 };
    }
    Line(Vector2 start, Vector2 end) {
        if (start.X > end.X) {
            this->end = start;
            this->start = end;
        }
        else {
            this->start = start;
            this->end = end;
        }
    }
    bool isOnLine(Vector2 pos) {
        double minX = std::min(start.X, end.X);
        double maxX = std::max(start.X, end.X);
        double minY = std::min(start.Y, end.Y);
        double maxY = std::max(start.Y, end.Y);
        if (pos.X >= minX && pos.X <= maxX && pos.Y >= minY && pos.Y <= maxY) return true;
        return false;
    }
    void draw() {
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(Main::renderer, &r, &g, &b, &a);
        SDL_SetRenderDrawColor(Main::renderer, 0, 0, 0, 255);
        iVector2 rstart= Main::convertWorldPosToCameraPos(start);
        iVector2 rend = Main::convertWorldPosToCameraPos(end);
        SDL_RenderDrawLine(Main::renderer, rstart.X, rstart.Y, rend.X, rend.Y);
        SDL_SetRenderDrawColor(Main::renderer, r,g,b,a);
    }

    bool checkIfIntersects(Line line) {
        Vector2 res = Main::getIntersectPt(*this, line);
        // std::cout<<res.X<<" "<<res.Y<<" "<<start.X<<" "<<start.Y<<" "<<end.X<<" "<<end.Y<<"\n";
        if (this->isOnLine(res) && line.isOnLine(res)) {
            return true;
        }
        return false;
    }
};