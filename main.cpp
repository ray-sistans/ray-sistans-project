#include <iostream>
#include <vector>
#include <cfloat>      
#include <limits>      
#include <algorithm>   
#include <string>
#include "src/raymath/Color.hpp"
#include "src/rayimage/Image.hpp"
#include "src/raymath/Vector3.hpp"
#include "src/raymath/Ray.hpp"
#include "src/raymath/Camera.hpp"
#include "src/raymath/Light.hpp"
#include "src/rayobject/Sphere.hpp"
#include "src/rayobject/Plane.hpp"


using namespace std;
Color CastRay(const Ray &ray, 
              const std::vector<Sphere> &spheres,
              const Plane& floor,
              const Light& light)
{
    float nearestObject = 99999.0f; // Start with "infinity"
    bool didHit = false;
    
    Vector3 hitPoint;   // Final intersection point of the *closest* object
    Color   hitColor;   // Color of the *closest* object
    Vector3 hitNormal;  // Surface normal of the *closest* object (for lighting)

    Vector3 tempHitPoint; // Temporary variable for intersection tests

    // Loop 1: Check all spheres
    for (const auto &sphere : spheres)
    {
        // Check if the ray intersects this specific sphere
        if (sphere.intersect(ray, tempHitPoint))
        {
            // Calculate distance from camera to the hit point
            float objectDistance = (tempHitPoint - ray.origin).Length();
            
            // If this hit is closer than any we've seen before...
            if (objectDistance < nearestObject) 
            {
                // ...update our record of the closest object
                nearestObject = objectDistance;
                hitPoint = tempHitPoint;
                hitColor = sphere.color;
                // Calculate the sphere's normal at the hit point
                hitNormal = (hitPoint - sphere.center).Normalized(); 
                didHit = true;
            }
        }
    }

    // Test 2: Check the floor
    if (floor.intersect(ray, tempHitPoint))
    {
        float objectDistance = (tempHitPoint - ray.origin).Length();
        
        // If the floor is even closer than the closest sphere...
        if (objectDistance < nearestObject) 
        {
            // ...update our record. The floor is now the closest object.
            nearestObject = objectDistance;
            hitPoint = tempHitPoint;
            hitColor = floor.color;
            hitNormal = floor.normal; // Use the plane's constant normal
            didHit = true;
        }
    }

    // This part only runs if the ray actually hit something (a sphere OR the floor)
    if (didHit)
    {
        // Calculate the direction vector from the hit point *to* the light source
        Vector3 lightDir = (light.position - hitPoint).Normalized();

        // Calculate the diffuse intensity using Lambert's cosine law (dot product)
        // std::max(0.0f, ...) ensures the light is not "negative" if it's behind the surface
        float diffuse = std::max(0.0f, hitNormal * lightDir);
        
        // Final color = Ambient light + Diffuse light
        // We simulate a 10% ambient light (0.1f) so shadows aren't pure black,
        // and 90% diffuse light (0.9f).
        return hitColor * (0.1f + 0.9f * diffuse);
    }
    
    // If the ray hit nothing (didHit == false), return the sky color
    return Color(0, 0.8, 0.8); // Cyan background
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
    // A dark green floor at Y = -2.5, facing upwards (normal = 0,1,0)
    Plane floor(Vector3(0, -2.5, 0), Vector3(0, 1, 0), Color(0.1, 0.3, 0.1));

    // Create a dynamic list of spheres
    std::vector<Sphere> spheres;
    
    // Add spheres (position, radius, color)
    spheres.push_back(Sphere(Vector3(0, 0, 20), 1.0f, Color(1, 0, 0)));     // Red sphere (center)
    spheres.push_back(Sphere(Vector3(-5, 1, 20), 1.0f, Color(0, 1, 0)));    // Green sphere (left)
    spheres.push_back(Sphere(Vector3(5, -2, 20), 1.0f, Color(0, 0, 1)));    // Blue sphere (right)
    spheres.push_back(Sphere(Vector3(-2, -1.5, 10), 1.0f, Color(1, 1, 0))); // Yellow sphere (front-left)
    spheres.push_back(Sphere(Vector3(-2, -1, 15), 1.0f, Color(1, 0, 1)));   // Purple sphere (mid-left)

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
            // This is where all the hard work happens!
            Color color = CastRay(ray, spheres, floor, light);

            // 3. Set the pixel in the final image
            image.SetPixel(x, y, color);
        }
    }

    cout << "Progress: 100%" << endl;
    image.WriteFile("output.png");
    cout << "Done! Image saved to output.png" << endl;

    return 0;
}

