#include <cmath> 
#include "Plane.hpp"

Plane::Plane(const Vector3& p, const Vector3& n, const Color& c)
    :point(p), normal(n.Normalized()), color(c)
    {

    }

    bool Plane::intersect(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
        float denom = normal * r.direction;
        if (std::abs(denom) > 1e-6) {
            float t = ((point - r.origin) * normal) / denom;
            if (t > t_min && t < t_max) {
                rec.t = t;
                rec.point = r.pointAt(t);
                rec.normal = normal;
                rec.color = color;
                return true;
            }
        }
        return false;
    }