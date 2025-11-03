#pragma once

#include "Vector3.hpp"

class Ray
{
public:
    Vector3 origin;
    Vector3 direction; // Normalized direction

    Ray();
    Ray(const Vector3 &o, const Vector3 &d);

    Vector3 pointAt(float t) const;
};