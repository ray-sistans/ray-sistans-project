#pragma once

#include "Ray.hpp"

class Camera
{
public:
    Vector3 position;
    Vector3 direction;
    int width;
    int height;

    Camera(int w, int h);
    Camera(const Vector3 &pos, const Vector3 &dir, int w, int h);

    Ray getRay(int x, int y) const;
};