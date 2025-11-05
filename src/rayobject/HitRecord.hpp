#pragma once
#include "../raymath/Vector3.hpp"
#include "../raymath/Color.hpp"

struct HitRecord {
    Vector3 point;
    Vector3 normal;
    float t;
    Color color;
};
