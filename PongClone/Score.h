#pragma once

#include <string>
#include <SDL_ttf.h>
#include <SDL_render.h>
#include "Vec2.h"

const std::string FONT_PATH = "fonts\\RobotoMono\\RobotoMono-Medium.ttf";

class Score
{
public:
    Score(Vec2 position, SDL_Renderer* renderer, TTF_Font* font);
    ~Score();
    
    void Draw();
    void SetScore(int score);

    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Surface* surface{};
    SDL_Texture* texture{};
    SDL_Rect rect{};
};
