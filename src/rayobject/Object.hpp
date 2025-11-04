#pragma once
#include "../raymath/Ray.hpp"
#include "../raymath/Color.hpp"
#include "../raymath/Vector3.hpp"

class Object {
public:
    virtual ~Object() {}
    virtual bool intersect(const Ray& ray, Vector3& intersectionPoint) const = 0;
    virtual Color getColor() const = 0;
    virtual Vector3 getNormal(const Vector3& point) const = 0;
};