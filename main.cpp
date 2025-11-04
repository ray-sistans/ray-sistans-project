#include <iostream>
#include <vector>
#include <cfloat>
#include <limits>
#include <algorithm>
#include <string>
#include <chrono>
#include "Scene.hpp"
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

int main()
{
    const int width = 1920;
    const int height = 1080;
    const int samplesPerPixel = 1;

    const auto start = chrono::steady_clock::now();

    const Light light = Light(Vector3(0, 10, 10), Color(1, 1, 1));
    Image image(width, height, Color(0, 0, 0));
    Camera camera(width, height);

    vector<Object *> objects;
    objects.push_back(new Sphere(Vector3(0, 0, 20), 1.0f, Color(1, 0, 0)));     // Red
    objects.push_back(new Sphere(Vector3(-5, 1, 20), 1.0f, Color(0, 1, 0)));    // Green
    objects.push_back(new Sphere(Vector3(5, -2, 20), 1.0f, Color(0, 0, 1)));    // Blue
    objects.push_back(new Sphere(Vector3(-2, -1.5, 10), 1.0f, Color(1, 1, 0))); // Yellow
    objects.push_back(new Sphere(Vector3(-2, -1, 15), 1.0f, Color(1, 0, 1)));   // Purple
    objects.push_back(new Plane(Vector3(0, -2.5, 0), Vector3(0, 1, 0), Color(0.8, 0.8, 0.8)));

    Scene scene(objects, light);

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
                pixelColor += scene.castRay(ray);
            }

            // 5. Faire la MOYENNE de toutes les couleurs
            Color finalColor = pixelColor / (float)samplesPerPixel;

            // 6. CLAMPER la couleur finale
            finalColor.clamp(0.0f, 0.999f);

            image.SetPixel(x, y, finalColor);
        }
    }

    const auto end = chrono::steady_clock::now();

    cout << "Progress: 100%  built in " << chrono::duration_cast<chrono::seconds>(end - start).count() << "s." << endl;
    image.WriteFile("output.png");

    for (auto obj : objects)
        delete obj;

    cout << "Done! Image saved to output.png" << endl;

    return 0;
}
