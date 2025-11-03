#include "Sphere.hpp"
#include <cmath>

Sphere::Sphere()
    : center(0, 0, 0), radius(1.0f), color(1, 1, 1) {}

Sphere::Sphere(const Vector3 &c, float r, const Color &col)
    : center(c), radius(r), color(col) {}

bool Sphere::intersect(const Ray &ray, Vector3 &intersectionPoint) const
{
    // Calculate vector from ray origin to sphere center
    //oc = origin - center of the sphere
    Vector3 oc = center - ray.origin;

    // Calculate dot product
    float dotProd = oc * ray.direction;

    // If negative, sphere is behind the ray
    if (dotProd < 0)
    {
        return false;
    }

    // Calculate point P on ray closest to sphere center
    //op = Multiply the dot product with the ray's direction vector
    Vector3 op = ray.direction * dotProd;
    Vector3 p = ray.origin + op;

    // Calculate distance from P to sphere center
    // cp = Displacement vector from c to p
    Vector3 cp = p - center;
    float lenCP = cp.Length();

    if (lenCP > radius)
    {
        return false;
    }

    // Calculate exact intersection point
    float a = std::sqrt(radius * radius - lenCP * lenCP);
    intersectionPoint = p + (ray.direction * (-a));

    return true;
}