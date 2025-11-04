#include "Camera.hpp"
#include "../raymath/Utils.hpp" 

Camera::Camera(int w, int h)
    : image_width(w), image_height(h)
{
    Vector3 pos(0, 0, 0);
    Vector3 look_dir(0, 0, 1);
    initialize(pos, look_dir);
}

Camera::Camera(const Vector3 &pos, const Vector3 &dir, int w, int h)
    : image_width(w), image_height(h)
{
    initialize(pos, dir.Normalized());
}

void Camera::initialize(const Vector3& pos, const Vector3& look_dir) {
    position = pos;

    float aspect_ratio = (float)image_width / (float)image_height;

    // Déterminer les dimensions du viewport
    // normY va de -1 à 1, donc la hauteur est de 2.0
    float viewport_height = 2.0f; 
    float viewport_width = viewport_height * aspect_ratio;
    
    // "focal_length" est la distance Z (4.0f) de votre ancienne logique
    float focal_length = 4.0f; 

    // Calculer les vecteurs de base (u, v, w) pour l'orientation de la caméra
    Vector3 w = look_dir.Normalized();
    // u = "droite" (perpendiculaire à 'w' et au 'up' (0,1,0))
    Vector3 u = Vector3(0, 1, 0).Cross(w).Normalized();
    // v = "haut" (perpendiculaire à 'w' et 'u')
    Vector3 v = w.Cross(u);

    // Vecteurs de déplacement sur le viewport
    Vector3 viewport_u = u * viewport_width;
    Vector3 viewport_v = v * -viewport_height; // v va vers le haut, mais nos pixels Y vont vers le bas

    // Déplacements par pixel
    pixel_delta_u = viewport_u / (float)image_width;
    pixel_delta_v = viewport_v / (float)image_height;

    // Trouver le point de départ : le pixel (0, 0)
    // (C'est le centre du viewport - la moitié des déplacements)
    Vector3 viewport_upper_left = pos + (w * focal_length) - (viewport_u / 2.0f) - (viewport_v / 2.0f);
    
    // On centre sur le pixel (0,0) 
    pixel00_loc = viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5f;
}

    // Retourne un vecteur aléatoire dans le carré [-0.5, +0.5]
    Vector3 Camera::sample_square() const {
    // Retourne un point aléatoire dans le carré unité [-0.5, -0.5] à [+0.5, +0.5]
    return Vector3(randomFloat() - 0.5f, randomFloat() - 0.5f, 0);
}

    // Construit un rayon pour un pixel (x, y) avec décalage aléatoire

    Ray Camera::getRay(int x, int y) const {
    
    // Obtenir un décalage aléatoire DANS le pixel
    Vector3 offset = sample_square();
    
    // Trouver la position 3D de ce point aléatoire
    Vector3 pixel_sample = pixel00_loc 
                         + ((float)x + offset.x) * pixel_delta_u
                         + ((float)y + offset.y) * pixel_delta_v;

    // Créer le rayon
    Vector3 ray_origin = position;
    Vector3 ray_direction = (pixel_sample - ray_origin).Normalized();
    
    return Ray(ray_origin, ray_direction);
}

