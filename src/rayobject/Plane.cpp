#include <cmath> 
#include "Plane.hpp"

Plane::Plane(const Vector3& pointOnPlane, const Vector3& normalVector, const Color& planeColor)
    :point(pointOnPlane), normal(normalVector.Normalized()), color(planeColor)
    {

    }

    bool Plane::intersect(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const {
        float denominator = normal * ray.direction;
        if (std::abs(denominator) > 1e-6) { // Check if the ray is not parallel to the plane
            float intersectionT = ((point - ray.origin) * normal) / denominator;
            if (intersectionT > tMin && intersectionT < tMax) {
                hitRecord.t = intersectionT;
                hitRecord.point = ray.pointAt(intersectionT);
                hitRecord.normal = normal;
                hitRecord.color = color;
                return true;
            }
        }
        return false;
    }