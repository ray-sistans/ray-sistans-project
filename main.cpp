#include <iostream>
#include <vector>
#include <cfloat>      
#include <limits>      
#include <algorithm>   
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
    
    float closest_distance = std::numeric_limits<float>::max(); 
    bool didHit = false;
    
    Vector3 hitPoint;   
    Color   hitColor;   
    Vector3 hitNormal;  

    Vector3 tempHitPoint;
    for (const auto &sphere : spheres)
    {
        if (sphere.intersect(ray, tempHitPoint))
        {
            float dist = (tempHitPoint - ray.origin).Length();
            if (dist < closest_distance) 
            {
                closest_distance = dist;
                hitPoint = tempHitPoint;
                hitColor = sphere.color;
                hitNormal = (hitPoint - sphere.center).Normalized(); 
                didHit = true;
            }
        }
    }
    if (floor.intersect(ray, tempHitPoint))
    {
        float dist = (tempHitPoint - ray.origin).Length();
        if (dist < closest_distance) 
        {
            closest_distance = dist;
            hitPoint = tempHitPoint;
            hitColor = floor.color;
            hitNormal = floor.normal; 
            didHit = true;
        }
    }

    if (didHit)
    {
        
        Vector3 lightDir = (light.position - hitPoint).Normalized();

        float diffuse = std::max(0.0f, hitNormal * lightDir);
        
        return hitColor * (0.1f + 0.9f * diffuse);
    }
    return Color(0, 0.8, 0.8);
}


int main()
{
    const int width = 1920;
    const int height = 1080;

    const Light light = Light(Vector3(0, 10, 10), Color(1, 1, 1));

    Image image(width, height, Color(0, 0, 0));

    Camera camera(width, height);

    Plane floor(Vector3(0, -2.5, 0), Vector3(0, 1, 0), Color(0.1, 0.3, 0.1));

    std::vector<Sphere> spheres;
    spheres.push_back(Sphere(Vector3(0, 0, 20), 1.0f, Color(1, 0, 0)));
    spheres.push_back(Sphere(Vector3(-5, 1, 20), 1.0f, Color(0, 1, 0)));
    spheres.push_back(Sphere(Vector3(5, -2, 20), 1.0f, Color(0, 0, 1)));
    spheres.push_back(Sphere(Vector3(-2, -1.5, 10), 1.0f, Color(1, 1, 0)));
    spheres.push_back(Sphere(Vector3(-2, -1, 15), 1.0f, Color(1, 0, 1)));

    cout << "Rendering " << width << "x" << height << " image..." << endl;

    for (int y = 0; y < height; ++y)
    {
        if (y % 50 == 0)
        {
            cout << "Progress: " << (y * 100 / height) << "%" << endl;
        }

        for (int x = 0; x < width; ++x)
        {
            Ray ray = camera.getRay(x, y);

            Color color = CastRay(ray, spheres, floor, light);

            image.SetPixel(x, y, color);
        }
    }

    cout << "Progress: 100%" << endl;
    image.WriteFile("output.png");
    cout << "Done! Image saved to output.png" << endl;

    return 0;
}