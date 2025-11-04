#pragma once

#include "../raymath/Ray.hpp"
#include "../raymath/Vector3.hpp"

class Camera
{
public:
    Camera(int w, int h);
    Camera(const Vector3 &pos, const Vector3 &dir, int w, int h);
    Ray getRay(int x, int y) const;

private:
    int image_width;
    int image_height;
    Vector3 position;
    Vector3 pixel00_loc;      // Emplacement du pixel (0, 0)
    Vector3 pixel_delta_u;    // Décalage vers le pixel de droite (x+1)
    Vector3 pixel_delta_v;    // Décalage vers le pixel du bas (y+1)

    //initialize grid of pixels
    void initialize(const Vector3& pos, const Vector3& look_dir);
    // Retourne un vecteur aléatoire dans le carré [-0.5, +0.5]
    Vector3 sample_square() const;
};

