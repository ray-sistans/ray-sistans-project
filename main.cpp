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


using namespace std;

Vector3 Reflect(const Vector3& direction, const Vector3& normal)
{
    //Formula : reflect = direction - 2(direction * normal)normal
    float projection = direction * normal;  // Produit scalaire
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

    Vector3 tempHitPoint; // Temporary variable for intersection tests

    // Loop 1: Find the closest sphere
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
        float intensity = hitNormal * lightDir;
        
        // Final color = Ambient light + Diffuse light
        Color finalColor = hitColor * (0.1f + 0.9f * intensity);
        
        if (reflect < 4)
        {
            Vector3 reflectDir = Reflect(ray.direction, hitNormal);

            Ray reflectRay(hitPoint + hitNormal * 0.001f, reflectDir);
            
            // cast the reflection ray
            Color reflectedColor = CastRay(reflectRay, objects, light, reflect + 1);

            // intensity of the reflection
            finalColor = finalColor + 0.5f * reflectedColor;
        }
        return finalColor;
    }
    
    // If the ray hit nothing (didHit == false)
    return Color(0, 0, 0); 
}


int main()
{
    const int width = 1920;
    const int height = 1080;

    // Define a single point light source (position, color)
    const Light light = Light(Vector3(0, 10, 10), Color(1, 1, 1)); // Bright white light

    Image image(width, height, Color(0, 0, 0)); // Final image buffer

    Camera camera(width, height);

    // Define the floor plane (position on plane, normal vector, color)
    // Create a dynamic list of spheres (Polymorphic Object pointers)
    std::vector<Object*> objects;
    
    // Add spheres (position, radius, color)
    objects.push_back(new Sphere(Vector3(0, 0, 20), 1.0f, Color(1, 0, 0)));     // Red sphere (center)
    objects.push_back(new Sphere(Vector3(-5, 1, 20), 1.0f, Color(0, 1, 0)));    // Green sphere (left)
    objects.push_back(new Sphere(Vector3(5, -2, 20), 1.0f, Color(0, 0, 1)));    // Blue sphere (right)
    objects.push_back(new Sphere(Vector3(-2, -1.5, 10), 1.0f, Color(1, 1, 0))); // Yellow sphere (front-left)
    objects.push_back(new Sphere(Vector3(-2, -1, 15), 1.0f, Color(1, 0, 1)));   // Purple sphere (mid-left)
    objects.push_back(new Plane(Vector3(0, -2.5, 0), Vector3(0, 1, 0), Color(0.8, 0.8, 0.8)));

    cout << "Rendering " << width << "x" << height << " image..." << endl;

    // Main Render Loop
    // Iterate over every pixel in the image
    for (int y = 0; y < height; ++y)
    {
        // Print progress
        if (y % 50 == 0)
        {
            cout << "Progress: " << (y * 100 / height) << "%" << endl;
        }

        for (int x = 0; x < width; ++x)
        {
            // 1. Get the ray for this specific pixel (from camera)
            Ray ray = camera.getRay(x, y);

            // 2. Trace the ray into the scene to get its color
            Color color = CastRay(ray, objects, light);

            // 3. Set the pixel in the final image
            image.SetPixel(x, y, color);
        }
    }

    cout << "Progress: 100%" << endl;
    image.WriteFile("output.png");

    for (auto obj : objects) delete obj;

    cout << "Done! Image saved to output.png" << endl;

    return 0;
}

