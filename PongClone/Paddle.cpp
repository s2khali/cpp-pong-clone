﻿#include <iostream>

#include "main.h"
#include "Paddle.h"

Paddle::Paddle(Vec2 position, Vec2 volatile, int id) : id(id), position(position), velocity(velocity)
{
    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);
    rect.w = PADDLE_WIDTH;
    rect.h = PADDLE_HEIGHT;
}

void Paddle::Draw(SDL_Renderer* renderer)
{
    rect.y = static_cast<int>(position.y);

    SDL_RenderFillRect(renderer, &rect);
}

void Paddle::Update(float dt)
{
    position += velocity * dt;

    if(position.y < 0)
    {
        position.y = 0;
    }
    else if (position.y > (WINDOW_HEIGHT - PADDLE_HEIGHT))
    {
        position.y = WINDOW_HEIGHT - PADDLE_HEIGHT;
    }
}

void Paddle::SetPosition(Vec2 _position)
{
    position = _position;
}


