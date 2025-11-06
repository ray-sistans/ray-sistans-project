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
        // For matte objects, use diffuse lighting
        if (closestHitRecord.material->reflectivity < 0.01f)
        {
            // Simple directional light from top
            Vector3 lightDir = Vector3(0.3f, 1.0f, -0.5f).Normalized();
            float diffuse = std::max(0.0f, closestHitRecord.normal * lightDir);

            // Ambient + diffuse
            float ambient = 0.2f;
            float lighting = ambient + (1.0f - ambient) * diffuse;

            return closestHitRecord.material->color * lighting;
        }
        Color finalColor = closestHitRecord.material->color * 0.3f; // Base color contribution

        if (recursionDepth < 4)
        {
            // Calculate perfect reflection
            Vector3 reflected = reflect(ray.direction, closestHitRecord.normal).Normalized();

            // Add randomness for brushed metal
            float fuzziness = closestHitRecord.material->fuzziness;
            Vector3 scattered = (reflected + randomInUnitSphere() * fuzziness).Normalized();

            // Use the SCATTERED direction, not the perfect reflection
            Ray reflectionRay(closestHitRecord.point + closestHitRecord.normal * 0.001f, scattered);

            Color reflectedColor = castRay(reflectionRay, recursionDepth + 1);

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
