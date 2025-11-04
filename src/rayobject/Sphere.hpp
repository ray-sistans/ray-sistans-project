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

    bool intersect(const Ray &ray, float t_min, float t_max, HitRecord& rec) const override;
};