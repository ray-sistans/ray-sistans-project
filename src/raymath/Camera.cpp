
#include "Camera.hpp"

Camera::Camera(int w, int h)
    : position(0, 0, 0), direction(0, 0, 1), width(w), height(h) {}

Camera::Camera(const Vector3 &pos, const Vector3 &dir, int w, int h)
    : position(pos), direction(dir.Normalized()), width(w), height(h) {}

Ray Camera::getRay(int x, int y) const
{
    float aspectRatio = static_cast<float>(width) / height;

    // Map pixel coordinates to normalized coordinates [-1, 1]
    float normX = (2.0f * x / width - 1.0f) * aspectRatio;

    float normY = 1.0f - 2.0f * y / height;

    // Create point on virtual screen (4 units away in z direction to prevent distortion on the sides)
    Vector3 screenPoint(normX, normY, position.z + 4.0f);

    // Direction from camera to screen point
    Vector3 rayDirection = (screenPoint - position).Normalized();

    return Ray(position, rayDirection);
}
