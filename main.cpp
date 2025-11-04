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
#include "HitRecord.hpp"


using namespace std;

Vector3 Reflect(const Vector3& direction, const Vector3& normal)
{
    //Formula : reflect = direction - 2(direction * normal)normal
    float projection = direction * normal;  // Produit scalaire
    return direction - 2.0f * projection * normal;
}

Color castRay(const Ray &ray, const std::vector<Object*>& objects, const Light& light, int recursionDepth = 0){
    HitRecord closestHitRecord;
    float closestIntersection = FLT_MAX;
    bool didHit = false;

    for (const auto &obj : objects) {
        HitRecord tempHitRecord;
        if (obj->intersect(ray, 0.001f, closestIntersection, tempHitRecord)) {
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
        float diffuseIntensity = closestHitRecord.normal * lightDirection;
        
        // Final color = Ambient light + Diffuse light
        Color finalColor = closestHitRecord.color * (0.1f + 0.9f * diffuseIntensity);
        
        if (recursionDepth < 4)
        {
            Vector3 reflectionDirection = Reflect(ray.direction, closestHitRecord.normal).Normalized();

            Ray reflectionRay(closestHitRecord.point + closestHitRecord.normal * 0.001f, reflectionDirection);
            
            // cast the reflection ray
            Color reflectedColor = castRay(reflectionRay, objects, light, recursionDepth + 1);

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
            Color color = castRay(ray, objects, light);

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
