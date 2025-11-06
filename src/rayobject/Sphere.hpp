#pragma once

#include "../raymath/Vector3.hpp"
#include "../raymath/Color.hpp"
#include "../raymath/Material.hpp"
#include "../raymath/Ray.hpp"
#include "Object.hpp"

class Sphere : public Object
{
public:
    Vector3 center;
    float radius;
    Material material;

    // Constructors
    Sphere();
    Sphere(const Vector3 &c, float r, const Material &m);

    bool intersect(const Ray &ray, float tMin, float tMax, HitRecord &rec) const override;
};