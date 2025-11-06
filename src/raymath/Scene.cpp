#include "Scene.hpp"
#include "../raymath/Utils.hpp"
#include "Material.hpp"

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
        float diffuseIntensity = std::max(0.0f, closestHitRecord.normal * lightDirection);

        // Final color = base color with 0.2 ambient light and 0.8 diffuse light
        Color finalColor = closestHitRecord.material->color * (0.2f + 0.8f * diffuseIntensity) * light.color;

        // For matte materials, return diffuse lighting
        if (closestHitRecord.material->reflectivity < 0.01f)
        {
            return finalColor;
        }

        // Handle reflections for reflective materials
        if (recursionDepth < 4)
        {
            // Calculate perfect reflection
            Vector3 reflectionDirection = reflect(ray.direction, closestHitRecord.normal).Normalized();

            // Add noise: randomize reflections according to fuzziness
            float fuzziness = closestHitRecord.material->fuzziness;
            Vector3 scatteredDirection = (reflectionDirection + randomInUnitSphere() * fuzziness).Normalized();

            Ray reflectionRay(closestHitRecord.point + closestHitRecord.normal * 0.001f, scatteredDirection);

            Color reflectedColor = castRay(reflectionRay, recursionDepth + 1);

            reflectedColor = reflectedColor * 0.9f; // dim reflections to prevent over reflections

            // Add the reflection to the base color
            finalColor = finalColor + closestHitRecord.material->reflectivity * reflectedColor;
        }

        return finalColor;
    }

    return Color(0, 0, 0); // Background color
}

Vector3 Scene::reflect(const Vector3 &direction, const Vector3 &normal)
{
    // Formula : reflect = direction - 2(direction * normal)normal
    float projection = direction * normal;
    return direction - 2.0f * projection * normal;
}
