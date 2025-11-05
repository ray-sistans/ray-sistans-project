
#pragma once

#include <vector>
#include <cfloat>
#include "Color.hpp"
#include "Vector3.hpp"
#include "Ray.hpp"
#include "Light.hpp"
#include "HitRecord.hpp"
#include "../rayobject/Object.hpp"

class Scene
{
public:
    std::vector<Object *> objects;
    Light light;

    Scene(const std::vector<Object *> &objs, const Light &l) : objects(objs), light(l) {}

    Color castRay(
        const Ray &ray,
        int reflect = 0);

    Vector3 reflect(const Vector3 &direction, const Vector3 &normal);
};