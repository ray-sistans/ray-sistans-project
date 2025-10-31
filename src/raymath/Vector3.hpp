#pragma once

#include <iostream>
#include <cmath>

/**
 * Classe Vector3 : Représente un vecteur ou un point dans l'espace 3D
 * Utilisée pour les positions, directions, normales, etc.
 */
class Vector3 {
public:
    float x, y, z;

    // Constructeurs
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    // Opérations vectorielles de base
    Vector3 operator+(const Vector3& v) const;
    Vector3 operator-(const Vector3& v) const;
    Vector3 operator*(float scalar) const;
    Vector3 operator/(float scalar) const;
    float operator*(const Vector3& v) const; // Produit scalaire (dot product)
    
    // Opérateurs d'assignation
    Vector3& operator+=(const Vector3& v);
    Vector3& operator-=(const Vector3& v);
    Vector3& operator*=(float scalar);
    Vector3& operator/=(float scalar);

    // Méthodes utilitaires
    float Length() const;           // Longueur du vecteur
    Vector3 Normalized() const;      // Retourne un vecteur normalisé
    
    // Produit vectoriel (cross product)
    Vector3 Cross(const Vector3& v) const;
    
    // Affichage
    friend std::ostream& operator<<(std::ostream& os, const Vector3& v);
};

// Opérateur pour scalar * vector
Vector3 operator*(float scalar, const Vector3& v);
