#pragma once

#include "Object.hpp"
#include "../raymath/Vector3.hpp"
#include "../raymath/Material.hpp"

class Triangle : public Object {
public:
    Triangle(const Vector3& vertex0, const Vector3& vertex1, const Vector3& vertex2, const Material& material);
    bool intersect(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const override;

public:
    Vector3 vertex0, vertex1, vertex2;
    Material material;
};


