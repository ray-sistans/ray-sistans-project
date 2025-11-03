#include <cmath> 
#include "Plane.hpp"

Plane::Plane(const Vector3& p, const Vector3& n, const Color& c)
    :point(p), normal(n.Normalized()), color(c)
    {

    }

    bool Plane::intersect(const Ray& r, Vector3& intersectionPoint) const {

        float denom = r.direction * normal;

        if(std::abs(denom) < 1e-6) {
            return false;
        }

        float num = (point - r.origin) * normal;

        float t = num / denom;

        if (t < 1e-6){
            return false;
        }

        intersectionPoint = r.pointAt(t);

        return true;
    }