#include "Camera.hpp"
#include "../raymath/Utils.hpp" 

Camera::Camera(int w, int h)
    : imageWidth(w), imageHeight(h)
{
    Vector3 pos(0, 0, 0);
    Vector3 lookDir(0, 0, 1);
    initialize(pos, lookDir);
}

Camera::Camera(const Vector3 &pos, const Vector3 &dir, int w, int h)
    : imageWidth(w), imageHeight(h)
{
    initialize(pos, dir.Normalized());
}

void Camera::initialize(const Vector3& pos, const Vector3& lookDir) {
    position = pos;

    float aspect_ratio = (float)imageWidth / (float)imageHeight;

    // Déterminer les dimensions du viewport
    // normY va de -1 à 1, donc la hauteur est de 2.0
    float viewportHeight = 2.0f; 
    float viewportWidth = viewportHeight * aspect_ratio;
    
    // "focalLength" est la distance Z (4.0f) de votre ancienne logique
    float focalLength = 4.0f; 

    // Calculer les vecteurs de base (u, v, w) pour l'orientation de la caméra
    Vector3 w = lookDir.Normalized();
    // u = "droite" (perpendiculaire à 'w' et au 'up' (0,1,0))
    Vector3 u = Vector3(0, 1, 0).Cross(w).Normalized();
    // v = "haut" (perpendiculaire à 'w' et 'u')
    Vector3 v = w.Cross(u);

    // Vecteurs de déplacement sur le viewport
    Vector3 viewportU = u * viewportWidth;
    Vector3 viewportV = v * -viewportHeight; // v va vers le haut, mais nos pixels Y vont vers le bas

    // Déplacements par pixel
    pixelXincrement = viewportU / (float)imageWidth;
    pixelYIncrement = viewportV / (float)imageHeight;

    // Trouver le point de départ : le pixel (0, 0)
    // (C'est le centre du viewport - la moitié des déplacements)
    Vector3 viewportUpperLeft = pos + (w * focalLength) - (viewportU / 2.0f) - (viewportV / 2.0f);
    
    // On centre sur le pixel (0,0) 
    pixel00Loc = viewportUpperLeft + (pixelXincrement + pixelYIncrement) * 0.5f;
}

    // Retourne un vecteur aléatoire dans le carré [-0.5, +0.5]
    Vector3 Camera::sampleSquare() const {
    // Retourne un point aléatoire dans le carré unité [-0.5, -0.5] à [+0.5, +0.5]
    return Vector3(randomFloat() - 0.5f, randomFloat() - 0.5f, 0);
}

    // Construit un rayon pour un pixel (x, y) avec décalage aléatoire

    Ray Camera::getRay(int x, int y) const {
    
    // Obtenir un décalage aléatoire DANS le pixel
    Vector3 offset = sampleSquare();
    
    // Trouver la position 3D de ce point aléatoire
    Vector3 pixelSample = pixel00Loc 
                         + ((float)x + offset.x) * pixelXincrement
                         + ((float)y + offset.y) * pixelYIncrement;

    // Créer le rayon
    Vector3 rayOrigin = position;
    Vector3 rayDirection = (pixelSample - rayOrigin).Normalized();
    
    return Ray(rayOrigin, rayDirection);
}

