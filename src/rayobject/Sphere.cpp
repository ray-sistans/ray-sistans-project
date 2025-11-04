#include "Sphere.hpp"
#include <cmath>

Sphere::Sphere()
    : center(0, 0, 0), radius(1.0f), color(1, 1, 1) {}

Sphere::Sphere(const Vector3 &c, float r, const Color &col)
    : center(c), radius(r), color(col) {}

bool Sphere::intersect(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
    Vector3 oc = r.origin - center;
    float a = r.direction.LengthSq();
    float half_b = oc * r.direction;
    float c = oc.LengthSq() - radius*radius;

    float discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    float sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    float root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.point = r.pointAt(rec.t);
    Vector3 outward_normal = (rec.point - center) * (1.0f / radius);
    rec.normal = outward_normal;
    rec.color = color;

    return true;
}