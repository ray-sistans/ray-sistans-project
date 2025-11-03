#pragma once

#include "../raymath/Vector3.hpp"
#include "../raymath/Color.hpp"
#include "../raymath/Ray.hpp"

class Sphere
{
public:
    Vector3 center;
    float radius;
    Color color;

    // Constructors
    Sphere();
    Sphere(const Vector3 &c, float r, const Color &col);

    // Return true if the ray intersects the sphere
    bool intersect(const Ray &ray, Vector3 &intersectionPoint) const;
};