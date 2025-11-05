#pragma once

#include "../raymath/Vector3.hpp"
#include "../raymath/Ray.hpp"
#include "../raymath/Color.hpp"
#include "Object.hpp"

class Plane : public Object 
{
public:
    Plane(const Vector3& p, const Vector3& n, const Color& c);


    bool intersect(const Ray& r, float tMin, float tMax, HitRecord& rec) const override;

public:
    Vector3 point;
    Vector3 normal;
    Color color;

    float checkerSize;
};
