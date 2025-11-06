#include <cmath>
#include "Plane.hpp"

Plane::Plane(const Vector3 &pointOnPlane, const Vector3 &normalVector, const Material &planeMaterial)
    : point(pointOnPlane), normal(normalVector.Normalized()), material(planeMaterial)
{
    // Size of each checker square
    checkerSize = 3.0f;
}

bool Plane::intersect(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord) const
{
    float denominator = normal * ray.direction;

    // Vérifier si le rayon n'est pas parallèle au plan
    if (std::abs(denominator) > 1e-6)
    {
        float intersectionT = ((point - ray.origin) * normal) / denominator;

        if (intersectionT > tMin && intersectionT < tMax)
        {
            hitRecord.t = intersectionT;
            hitRecord.point = ray.pointAt(intersectionT);
            hitRecord.normal = normal;

            // get the hitRecord point coordinates
            float x = hitRecord.point.x;
            float y = hitRecord.point.y;
            float z = hitRecord.point.z;

            //    Si le plan est horizontal (normal pointe vers le haut), on utilise X et Z
            //    Si le plan est vertical, on utilise d'autres coordonnées
            float horizontal, vertical;

            // if plane is horizontal along Y (normal = (0,1,0) or close)
            if (std::abs(normal.y) > 0.9f)
            {
                horizontal = x; // Coordonnée horizontale
                vertical = z;   // Coordonnée de profondeur
            }
            // if plane is vertical along X (normal = (1,0,0) or close)
            else if (std::abs(normal.x) > 0.9f)
            {
                horizontal = y;
                vertical = z;
            }
            // if plane is vertical along Z (normal = (0,0,1) or close)
            else
            {
                horizontal = x;
                vertical = y;
            }

            // divide by checkerSize to get the number of checkers crossed
            int checkHorizontal = static_cast<int>(std::floor(horizontal / checkerSize));
            int checkVertical = static_cast<int>(std::floor(vertical / checkerSize));

            // pair = white, odd = black
            bool isWhite = (checkHorizontal + checkVertical) % 2 == 0;

            // 5. Appliquer la couleur correspondante
            if (isWhite)
            {
                hitRecord.material = new Material(Material::BrushedMetal(Color(0.6f, 0.6f, 0.6f)));
            }
            else
            {
                hitRecord.material = new Material(Material::BrushedMetal(Color(0.0f, 0.0f, 0.0f)));
            }

            return true;
        }
    }
    return false;
}