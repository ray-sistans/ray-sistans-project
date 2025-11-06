#include <iostream>
#include <vector>
#include <cfloat>
#include <limits>
#include <string>
#include <chrono>
#include <omp.h>
#include <thread>
#include "Scene.hpp"
#include "Image.hpp"
#include "Camera.hpp"
#include "Utils.hpp"
#include "Color.hpp"
#include "Light.hpp"
#include "HitRecord.hpp"
#include "Object.hpp"
#include "Utils.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Material.hpp"
#include "Triangle.hpp"

using namespace std;
using namespace std::chrono;

// Interrupteur 0 single thread 1 mt
#define MT 1

int main()
{
    const int width = 3840;
    const int height = 2160;
    const int samplesPerPixel = 1;

    const auto start = steady_clock::now();

    const Light light = Light(Vector3(0, 10, 10), Color(0.9f, 0.9f, 0.9f));
    Image image(width, height, Color(0, 0, 0));
    Camera camera(width, height);

    vector<Object *> objects;
    // spheres
    objects.push_back(new Sphere(light.position, 1.25f, Material(light.color, 1.0f, 0.0f)));                // Light
    objects.push_back(new Sphere(Vector3(0, 0, 20), 1.0f, Material::Mirror(Color(1.0, 0, 0))));             // Red
    objects.push_back(new Sphere(Vector3(-4, 3, 25), 1.0f, Material(Color(0.35, 0.26, 0.73), 0.9f, 0.2f))); // Purple
    objects.push_back(new Sphere(Vector3(-5, 1, 20), 1.0f, Material::Matte(Color(0.88, 0.33, 0.56))));      // Pink
    objects.push_back(new Sphere(Vector3(5, -2, 20), 1.0f, Material::Mirror(Color(0, 0, 1))));              // Blue
    objects.push_back(new Sphere(Vector3(5.5, -2.4, 18), 0.1f, Material::Mirror(Color(1, 1, 0))));          // Yellow
    objects.push_back(new Sphere(Vector3(-2, -1, 15), 1.0f, Material::Matte(Color(1, 0.58, 0.3))));         // Orange terracota
    objects.push_back(new Sphere(Vector3(5, -1, 15), 1.0f, Material::Mirror(Color(0, 0, 0))));              // Black mirror

    objects.push_back(new Sphere(Vector3(-2, -1.5, 10), 1.0f, Material::Gold()));                                   // Gold
    objects.push_back(new Sphere(Vector3(2, -1.5, 15), 1.0f, Material::Silver()));                                  // Silver
    objects.push_back(new Sphere(Vector3(0.75, -2.25, 16), 0.25f, Material(Color(0.95, 0.5, 0.3), 0.85f, 0.25f)));  // Copper
    objects.push_back(new Sphere(Vector3(1.0, -2.25, 12.5), 0.25f, Material(Color(0.2, 0.2, 0.22), 0.75f, 0.35f))); // Dark steel
    objects.push_back(new Sphere(Vector3(2.5, -2.25, 13), 0.25f, Material(Color(0.2, 0.9, 0.9), 0.75f, 0.35f)));    // Diamond
    objects.push_back(new Sphere(Vector3(5, -2.4, 12.5), 0.1f, Material::PolishedMetal(Color(0.0, 0.8, 0.5))));     // Emerald
    objects.push_back(new Sphere(Vector3(4.9, -2.4, 13), 0.1f, Material::PolishedMetal(Color(0.9, 0.1, 0.2))));     // Ruby
    // triangles
    objects.push_back(new Triangle(Vector3(-30, -5, 150), Vector3(30, -5, 150), Vector3(0, 30, 150), Material::Gold())); // Gold pyramid
    // plane
    objects.push_back(new Plane(Vector3(0, -2.5, 0), Vector3(0, 1, 0), Material::Mirror(Color(0.8, 0.8, 0.8))));

    Scene scene(objects, light);

    cout << "Rendering " << width << "x" << height << " with "
         << samplesPerPixel << " samples per pixel..." << endl;
#if MT
    cout << "Mode: Multithreading" << endl;
#else
    cout << "Mode: Single-threading" << endl;
#endif

#if MT
    omp_set_num_threads(8); // mon cpu fait 8 coeurs
#pragma omp parallel for schedule(static)
    for (int y = 0; y < height; ++y)
    {
        vector<Color> row(width); // buffer local pour la ligne

        for (int x = 0; x < width; ++x)
        {
            Color pixelColor(0, 0, 0);
            for (int s = 0; s < samplesPerPixel; ++s)
            {
                Ray ray = camera.getRay(x, y);
                pixelColor += scene.castRay(ray);
            }
            Color finalColor = pixelColor / (float)samplesPerPixel;
            finalColor.clamp(0.0f, 0.999f);
            row[x] = finalColor;
        }

        for (int x = 0; x < width; ++x)
            image.SetPixel(x, y, row[x]);

        if (y % 50 == 0)
        {
            {
                cout << "Progress: " << (int)((float)y / (float)height * 100.0f) << "%\r" << flush;
            }
        }
    }
    cout << endl;

#else
    // Version single-thread
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
                Ray ray = camera.getRay(x, y);
                pixelColor += scene.castRay(ray);
            }
            Color finalColor = pixelColor / (float)samplesPerPixel;
            finalColor.clamp(0.0f, 0.999f);
            image.SetPixel(x, y, finalColor);
        }
    }
#endif

    const auto end = steady_clock::now();
    const auto duration = duration_cast<seconds>(end - start).count();

    image.WriteFile("output.png");

    for (auto obj : objects)
        delete obj;

    cout << "Done! Image saved to output.png --- Rendered in " << duration << "s " << endl;
    image.WriteFile("output.png");
    return 0;
}
