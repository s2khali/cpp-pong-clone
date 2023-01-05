#pragma once

#include <SDL_render.h>
#include "Contact.h"
#include "Vec2.h"

const int BALL_WIDTH = 15;
const int BALL_HEIGHT = 15;

const float BALL_SPEED = 0.5f;
const float VERTICAL_SPEED = 0.75f;

class Ball
{
public:
    Ball(Vec2 position, Vec2 velocity);
    
    void Draw(SDL_Renderer* render);
    void Update(float dt);

    void CollideWithPaddle(Contact const& contact);
    void CollideWithWall(Contact const& contact);

    void SetPosition(Vec2 _position);
    void SetVelocity(Vec2 _velocity);

    Vec2 position;
    Vec2 velocity;
    SDL_Rect rect{};
};
