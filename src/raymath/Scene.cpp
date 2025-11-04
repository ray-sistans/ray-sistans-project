#include "Scene.hpp"

Color Scene::castRay(
    const Ray &ray,
    int reflect)
{
    float nearestObject = 99999.0f; // Start with "infinity"
    bool didHit = false;

    Vector3 hitPoint;  // Final intersection point of the *closest* object
    Color hitColor;    // Color of the *closest* object
    Vector3 hitNormal; // Surface normal of the *closest* object (for lighting)

    Vector3 tempHitPoint;

    for (const auto &obj : objects)
    {
        Vector3 tempHitPoint;
        if (obj->intersect(ray, tempHitPoint))
        {
            float objectDistance = (tempHitPoint - ray.origin).Length();
            if (objectDistance < nearestObject)
            {
                nearestObject = objectDistance;
                hitPoint = tempHitPoint;
                hitColor = obj->getColor();
                hitNormal = obj->getNormal(hitPoint);
                didHit = true;
            }
        }
    }

    if (didHit)
    {
        // Calculate the direction vector from the hit point *to* the light source
        Vector3 lightDir = (light.position - hitPoint).Normalized();

        // Calculate the diffuse intensity (dot product)
        float intensity = std::max(0.0f, hitNormal * lightDir); // intensité n'est pas négative

        // Ambient light + Diffuse light
        Color finalColor = hitColor * (0.1f + 0.9f * intensity);

        if (reflect < 4)
        {
            Vector3 reflectDir = Scene::reflect(ray.direction, hitNormal);
            // Décaler le point d'origine pour éviter l'auto-intersection
            Ray reflectRay(hitPoint + hitNormal * 0.001f, reflectDir);

            Color reflectedColor = Scene::castRay(reflectRay, reflect + 1);

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