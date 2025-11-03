#include "Vector3.hpp"

// Addition de deux vecteurs
Vector3 Vector3::operator+(const Vector3& v) const {
    return Vector3(x + v.x, y + v.y, z + v.z);
}

// Soustraction de deux vecteurs
Vector3 Vector3::operator-(const Vector3& v) const {
    return Vector3(x - v.x, y - v.y, z - v.z);
}

// Multiplication par un scalaire
Vector3 Vector3::operator*(float scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
}

// Division par un scalaire
Vector3 Vector3::operator/(float scalar) const {
    return Vector3(x / scalar, y / scalar, z / scalar);
}

// Produit scalaire (dot product)
// Utile pour calculer l'angle entre deux vecteurs et pour l'éclairage
float Vector3::operator*(const Vector3& v) const {
    return x * v.x + y * v.y + z * v.z;
}

// Opérateurs d'assignation
Vector3& Vector3::operator+=(const Vector3& v) {
    x += v.x; y += v.y; z += v.z;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& v) {
    x -= v.x; y -= v.y; z -= v.z;
    return *this;
}

Vector3& Vector3::operator*=(float scalar) {
    x *= scalar; y *= scalar; z *= scalar;
    return *this;
}

Vector3& Vector3::operator/=(float scalar) {
    x /= scalar; y /= scalar; z /= scalar;
    return *this;
}

// Calcule la longueur du vecteur
float Vector3::Length() const {
    return std::sqrt(x * x + y * y + z * z);
}

// Retourne une version normalisée du vecteur (longueur = 1)
Vector3 Vector3::Normalized() const {
    float len = Length();
    if (len > 0) {
        return *this / len;
    }
    return Vector3(0, 0, 0);
}

// Produit vectoriel (cross product)
// Retourne un vecteur perpendiculaire aux deux vecteurs
Vector3 Vector3::Cross(const Vector3& v) const {
    return Vector3(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x
    );
}

// Opérateur pour affichage
std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

// Opérateur externe pour scalar * vector
Vector3 operator*(float scalar, const Vector3& v) {
    return v * scalar;
}
