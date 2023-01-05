#include "Ball.h"

Ball::Ball(Vec2 position, Vec2 velocity) : position(position), velocity(velocity)
{
    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);
    rect.w = BALL_WIDTH;
    rect.h = BALL_HEIGHT;
}

void Ball::Update(float dt)
{
    position += velocity * dt;
}


void Ball::Draw(SDL_Renderer* renderer)
{
    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);

    SDL_RenderFillRect(renderer, &rect);
}

void Ball::CollideWithPaddle(Contact const& contact)
{
    position.x += contact.penetration;
    velocity.x = -velocity.x;

    if (contact.type == CollisionType::Top)
    {
        velocity.y = -VERTICAL_SPEED * BALL_SPEED;
    }
    else if (contact.type == CollisionType::Botttom)
    {
        velocity.y = VERTICAL_SPEED * BALL_SPEED;
    }
}

void Ball::CollideWithWall(Contact const& contact)
{
    position.y += contact.penetration;
    velocity.y = -velocity.y;
}

void Ball::SetPosition(Vec2 _position)
{
    position = _position;
}

void Ball::SetVelocity(Vec2 _velocity)
{
    velocity = _velocity;
}
