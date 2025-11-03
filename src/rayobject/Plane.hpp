#pragma once

#include "../raymath/Vector3.hpp"
#include "../raymath/Ray.hpp"

class Plane
{
public:
    Plane(const Vector3& p, const Vector3& n);


    bool intersect(const Ray& r, Vector3& intersectionPoint)const;

private:
    Vector3 point;
    Vector3 normal;

};
