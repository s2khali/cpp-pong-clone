﻿#include "Score.h"

Score::Score(Vec2 position, SDL_Renderer* renderer, TTF_Font* font)
    : renderer(renderer), font(font)
{
    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);
    
    SetScore(0);
}

Score::~Score()
{
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Score::Draw()
{
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

void Score::SetScore(int score)
{
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    surface = TTF_RenderText_Solid(font, std::to_string(score).c_str(), {0xFF, 0xFF, 0xFF, 0xFF});
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    rect.w = width;
    rect.h = height;
}


