#pragma once
#include "Vector3.hpp"
#include "Color.hpp"

class Material;
struct HitRecord
{
    Vector3 point;
    Vector3 normal;
    float t;
    Material *material;
};
