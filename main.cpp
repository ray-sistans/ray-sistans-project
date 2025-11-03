#include <iostream>
#include "Color.hpp"
#include "Image.hpp"
#include "Vector3.hpp"
#include "Ray.hpp"
#include "Camera.hpp"
#include "Sphere.hpp"

using namespace std;

// Cast a ray into the scene and return the color
Color CastRay(const Ray &ray, const std::vector<Sphere> &spheres)
{
  Color pixelColor(0, 0.8, 0.8);

  // Check intersection with each sphere
  for (const auto &sphere : spheres)
  {
    Vector3 intersectionPoint;

    if (sphere.intersect(ray, intersectionPoint))
    {
      pixelColor = sphere.color;
    }
  }

  return pixelColor;
}

int main()
{
  const int width = 1920;
  const int height = 1080;

  Image image(width, height, Color(0, 0, 0));

  Camera camera(width, height);

  // Sphere array
  std::vector<Sphere> spheres;

  // Red sphere in center
  spheres.push_back(Sphere(Vector3(0, 0, 20), 1.0f, Color(1, 0, 0)));

  // Green sphere left
  spheres.push_back(Sphere(Vector3(-5, 1, 20), 1.0f, Color(0, 1, 0)));

  // Blue sphere right
  spheres.push_back(Sphere(Vector3(5, -2, 20), 1.0f, Color(0, 0, 1)));

  // Yellow front left
  spheres.push_back(Sphere(Vector3(-2, -1.5, 10), 1.0f, Color(1, 1, 0)));

  cout << "Rendering " << width << "x" << height << " image..." << endl;

  // Render the image
  for (int y = 0; y < height; ++y)
  {
    if (y % 50 == 0)
    {
      cout << "Progress: " << (y * 100 / height) << "%" << endl;
    }

    for (int x = 0; x < width; ++x)
    {
      Ray ray = camera.getRay(x, y);

      Color color = CastRay(ray, spheres);

      image.SetPixel(x, y, color);
    }
  }

  cout << "Progress: 100%" << endl;

  image.WriteFile("output.png");

  cout << "Done! Image saved to output.png" << endl;

  return 0;
}