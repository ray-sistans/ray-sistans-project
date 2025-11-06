#include "Sphere.hpp"
#include <cmath>

Sphere::Sphere()
    : center(0, 0, 0), radius(1.0f), material(Color(1, 1, 1), 1.0f, 0.0f) {}

Sphere::Sphere(const Vector3 &c, float r, const Material &material)
    : center(c), radius(r), material(material) {}

bool Sphere::intersect(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord) const
{
    // Vector from ray origin to sphere center
    Vector3 originCenter = ray.origin - center;

    // Quadratic coefficients for ray-sphere intersection
    float rayDirectionLengthSq = ray.direction.LengthSq();
    float dotProduct = originCenter * ray.direction; // DOT PRODUCT
    float distanceToSphereSq = originCenter.LengthSq() - radius * radius;

    // Discriminant determines number of intersections
    float discriminant = dotProduct * dotProduct - rayDirectionLengthSq * distanceToSphereSq;

    if (discriminant < 0)
        return false; // Ray misses sphere

    float sqrtDiscriminant = sqrt(discriminant);

    // Find nearest intersection (entry point)
    float t = (-dotProduct - sqrtDiscriminant) / rayDirectionLengthSq;

    // If entry is out of range, check exit point
    if (t < tMin || tMax < t)
    {
        t = (-dotProduct + sqrtDiscriminant) / rayDirectionLengthSq;
        if (t < tMin || tMax < t)
            return false;
    }

    hitRecord.t = t;
    hitRecord.point = ray.pointAt(t);
    hitRecord.normal = (hitRecord.point - center) * (1.0f / radius);
    hitRecord.material = const_cast<Material *>(&material);

    return true;
}