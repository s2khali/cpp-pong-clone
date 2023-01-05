#pragma once
#include <SDL_render.h>

#include "Vec2.h"

const int PADDLE_WIDTH = 5;
const int PADDLE_HEIGHT = 55;

const float PADDLE_SPEED = 1.0f;

class Paddle
{
public:
    Paddle(Vec2 position, Vec2 velocity, int id);
    
    void Draw(SDL_Renderer* renderer);
    void Update(float dt);

    void SetPosition(Vec2 _position);

    int id;
    Vec2 position;
    Vec2 velocity;
    SDL_Rect rect{};
};
