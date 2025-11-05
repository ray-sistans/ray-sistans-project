#pragma once
#include "Vector3.hpp"
#include "Color.hpp"

struct HitRecord
{
    Vector3 point;
    Vector3 normal;
    float t;
    Color color;
};
