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
    int imageWidth;
    int imageHeight;
    Vector3 position;
    Vector3 pixel00Loc;      // Emplacement du pixel (0, 0)
    Vector3 pixelXincrement;    // Décalage vers le pixel de droite (x+1)
    Vector3 pixelYIncrement;    // Décalage vers le pixel du bas (y+1)

    //initialize grid of pixels
    void initialize(const Vector3& pos, const Vector3& lookDir);
    // Retourne un vecteur aléatoire dans le carré [-0.5, +0.5]
    Vector3 sampleSquare() const;
};

