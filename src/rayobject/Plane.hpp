#pragma once

#include "../raymath/Vector3.hpp"
#include "../raymath/Ray.hpp"
#include "../raymath/Color.hpp"

class Plane
{
public:
    Plane(const Vector3& p, const Vector3& n, const Color& c);


    bool intersect(const Ray& r, Vector3& intersectionPoint)const;

public:
    Vector3 point;
    Vector3 normal;
    Color color;

};
