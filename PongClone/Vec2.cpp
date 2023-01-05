﻿#include "Vec2.h"

Vec2::Vec2() : x(0.00f), y(0.00f)
{
}

Vec2::Vec2(float x, float y) : x(x), y(y)
{
}

Vec2 Vec2::operator+(Vec2 const& rhs)
{
    return Vec2(x + rhs.x, y + rhs.y);
}

Vec2& Vec2::operator+=(Vec2 const& rhs)
{
    x += rhs.x;
    y += rhs.y;

    return *this;
}

Vec2 Vec2::operator*(float rhs)
{
    return Vec2(x * rhs, y * rhs);
}
