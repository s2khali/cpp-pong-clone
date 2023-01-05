#pragma once

enum class CollisionType
{
    None,
    Top,
    Middle,
    Botttom,
    Left,
    Right
};

struct Contact
{
    CollisionType type;
    float penetration;
};