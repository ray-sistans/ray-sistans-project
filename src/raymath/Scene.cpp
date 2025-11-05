#include "Scene.hpp"

Color Scene::castRay(const Ray &ray, int recursionDepth)
{
    HitRecord closestHitRecord;
    float closestIntersection = FLT_MAX;
    bool didHit = false;

    for (const auto &obj : objects)
    {
        HitRecord tempHitRecord;
        if (obj->intersect(ray, 0.001f, closestIntersection, tempHitRecord))
        {
            didHit = true;
            closestIntersection = tempHitRecord.t;
            closestHitRecord = tempHitRecord;
        }
    }

    if (didHit)
    {
        // Calculate the direction vector from the hit point *to* the light source
        Vector3 lightDirection = (light.position - closestHitRecord.point).Normalized();

        // Calculate the diffuse intensity (dot product)
        float diffuseIntensity = std::max(0.0f, closestHitRecord.normal * lightDirection); // intensité n'est pas négative

        // Final color = Ambient light + Diffuse light
        Color finalColor = closestHitRecord.color * (0.1f + 0.9f * diffuseIntensity);

        if (recursionDepth < 4)
        {
            Vector3 reflectionDirection = reflect(ray.direction, closestHitRecord.normal).Normalized();

            Ray reflectionRay(closestHitRecord.point + closestHitRecord.normal * 0.001f, reflectionDirection);

            Color reflectedColor = castRay(reflectionRay, recursionDepth + 1);

            // intensity of the reflection (0.5 = 50% de réflectivité)
            finalColor = finalColor + 0.5f * reflectedColor;
        }
        return finalColor;
    }

    // (didHit == false)
    return Color(0, 0, 0);
}

Vector3 Scene::reflect(const Vector3 &direction, const Vector3 &normal)
{
    // Formula : reflect = direction - 2(direction * normal)normal
    float projection = direction * normal;
    return direction - 2.0f * projection * normal;
}
