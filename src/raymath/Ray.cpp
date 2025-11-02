#include "Ray.hpp"

Ray::Ray()
    : origin(0, 0, 0), direction(0, 0, 1) {}

Ray::Ray(const Vector3 &o, const Vector3 &d)
    : origin(o), direction(d.Normalized()) {}

Vector3 Ray::pointAt(float t) const
{
    return origin + direction * t;
}