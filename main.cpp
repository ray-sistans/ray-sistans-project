#include <iostream>
#include <vector>
#include <cfloat> 
#include <limits> 
#include <algorithm> 
#include <string>
#include "Color.hpp"
#include "Image.hpp"
#include "Vector3.hpp"
#include "Ray.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Object.hpp"
#include "Utils.hpp"    

using namespace std;

Vector3 Reflect(const Vector3& direction, const Vector3& normal)
{
    //Formula : reflect = direction - 2(direction * normal)normal
    float projection = direction * normal; 
    return direction - 2.0f * projection * normal;
}

Color CastRay(
  const Ray &ray, 
  const std::vector<Object*>& objects, 
  const Light& light, 
  int reflect = 0) 
  {
    float nearestObject = 99999.0f; // Start with "infinity"
    bool didHit = false;
    
    Vector3 hitPoint;   // Final intersection point of the *closest* object
    Color   hitColor;   // Color of the *closest* object
    Vector3 hitNormal;  // Surface normal of the *closest* object (for lighting)

    Vector3 tempHitPoint; 

    for (const auto& obj : objects) {
      Vector3 tempHitPoint;
      if (obj->intersect(ray, tempHitPoint)) {
          float objectDistance = (tempHitPoint - ray.origin).Length();
          if (objectDistance < nearestObject) {
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
            Vector3 reflectDir = Reflect(ray.direction, hitNormal);
            // Décaler le point d'origine pour éviter l'auto-intersection
            Ray reflectRay(hitPoint + hitNormal * 0.001f, reflectDir);
            
            Color reflectedColor = CastRay(reflectRay, objects, light, reflect + 1);

            // intensity of the reflection (0.5 = 50% de réflectivité)
            finalColor = finalColor + 0.5f * reflectedColor;
        }
        return finalColor;
    }
    
    // (didHit == false)
    return Color(0, 0, 0);
}


int main()
{
    const int width = 1920;
    const int height = 1080;
    const int samplesPerPixel = 10;

    const Light light = Light(Vector3(0, 10, 10), Color(1, 1, 1));
    Image image(width, height, Color(0, 0, 0));
    Camera camera(width, height); 

    std::vector<Object*> objects;
    objects.push_back(new Sphere(Vector3(0, 0, 20), 1.0f, Color(1, 0, 0)));    // Red
    objects.push_back(new Sphere(Vector3(-5, 1, 20), 1.0f, Color(0, 1, 0)));   // Green
    objects.push_back(new Sphere(Vector3(5, -2, 20), 1.0f, Color(0, 0, 1)));   // Blue
    objects.push_back(new Sphere(Vector3(-2, -1.5, 10), 1.0f, Color(1, 1, 0))); // Yellow
    objects.push_back(new Sphere(Vector3(-2, -1, 15), 1.0f, Color(1, 0, 1)));   // Purple
    objects.push_back(new Plane(Vector3(0, -2.5, 0), Vector3(0, 1, 0), Color(0.8, 0.8, 0.8)));

    cout << "Rendering " << width << "x" << height << " with " 
         << samplesPerPixel << " samples per pixel..." << endl;

    for (int y = 0; y < height; ++y)
    {
        if (y % 50 == 0)
        {
            cout << "Progress: " << (y * 100 / height) << "%" << endl;
        }

        for (int x = 0; x < width; ++x)
        {
            Color pixelColor(0, 0, 0);

            for (int s = 0; s < samplesPerPixel; ++s)
            {
                // un rayon aléatoire DANS ce pixel
                Ray ray = camera.getRay(x, y);
                
                // 4. Lancer le rayon et ACCUMULER la couleur
                pixelColor += CastRay(ray, objects, light);
            }
            
            // 5. Faire la MOYENNE de toutes les couleurs
            Color finalColor = pixelColor / (float)samplesPerPixel;

            // 6. CLAMPER la couleur finale 
            finalColor.clamp(0.0f, 0.999f); 
            
            image.SetPixel(x, y, finalColor);
        }
    }

    cout << "Progress: 100%" << endl;
    image.WriteFile("output.png");

    for (auto obj : objects) delete obj;

    cout << "Done! Image saved to output.png" << endl;

    return 0;
}

