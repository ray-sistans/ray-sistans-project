#pragma once

#include <iostream>
#include <cmath>

class Vector3 {
public:
    float x, y, z;

    //constructors
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    //all operations for Vector3
    Vector3 operator+(const Vector3& v) const; 
    Vector3 operator-(const Vector3& v) const; 
    Vector3 operator*(float t) const;
    float operator*(const Vector3& other) const; 

    // Opérateurs pour l'Anti-Aliasing
    Vector3& operator+=(const Vector3& v);
    Vector3 operator/(float t) const;
    
    //  Calculates the cross product (produit vectoriel)
    Vector3 Cross(const Vector3& v) const;

    float Length() const;                
    float LengthSq() const;
    Vector3 Normalized() const;            
};

//rayon = o + t*d
Vector3 operator*(float t, const Vector3& v);