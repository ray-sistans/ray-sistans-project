#pragma once
#include "../raymath/Ray.hpp"
#include "HitRecord.hpp"

class Object {
public:
    virtual ~Object() {}
    virtual bool intersect(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
};