#pragma once
#include "../raymath/Ray.hpp"
#include "../raymath/HitRecord.hpp"

class Object
{
public:
    virtual ~Object() {}
    virtual bool intersect(const Ray &r, float tMin, float tMax, HitRecord &rec) const = 0;
};