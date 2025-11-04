#pragma once

#include "../raymath/Vector3.hpp"
#include "../raymath/Color.hpp"
#include "../raymath/Ray.hpp"
#include "Object.hpp"

class Sphere : public Object
{
public:
    Vector3 center;
    float radius;
    Color color;

    // Constructors
    Sphere();
    Sphere(const Vector3 &c, float r, const Color &col);

    Color getColor() const override { return color; }
    Vector3 getNormal(const Vector3& point) const override { return (point - center).Normalized(); }
    // Return true if the ray intersects the sphere
    bool intersect(const Ray &ray, Vector3 &intersectionPoint) const override;
};