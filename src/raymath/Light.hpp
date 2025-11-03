#pragma once

#include "Vector3.hpp"
#include "Color.hpp"

class Light
{
public:
    Vector3 position;
    Color color;

    Light(Vector3 position, Color color = Color(1, 1, 1));
};