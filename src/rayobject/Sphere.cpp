#include "Sphere.hpp"
#include <cmath>

Sphere::Sphere()
    : center(0, 0, 0), radius(1.0f), color(1, 1, 1) {}

Sphere::Sphere(const Vector3 &c, float r, const Color &col)
    : center(c), radius(r), color(col) {}

bool Sphere::intersect(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const {
    Vector3 originToCenter = ray.origin - center;
    float quadA = ray.direction.LengthSq();
    float halfB = originToCenter * ray.direction;
    float quadC = originToCenter.LengthSq() - radius*radius;

    float discriminant = halfB*halfB - quadA*quadC;
    if (discriminant < 0) return false;
    float sqrtDiscriminant = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    float root = (-halfB - sqrtDiscriminant) / quadA;
    if (root < tMin || tMax < root) {
        root = (-halfB + sqrtDiscriminant) / quadA;
        if (root < tMin || tMax < root)
            return false;
    }

    hitRecord.t = root;
    hitRecord.point = ray.pointAt(hitRecord.t);
    Vector3 outwardNormal = (hitRecord.point - center) * (1.0f / radius);
    hitRecord.normal = outwardNormal;
    hitRecord.color = color;

    return true;
}