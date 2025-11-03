#include "Vector3.hpp"

//addition point + Direction = new point
Vector3 Vector3::operator+(const Vector3& v) const {
    return Vector3(x + v.x, y + v.y, z + v.z);
}

//subtraction calculates the displacement: Target - Start
Vector3 Vector3::operator-(const Vector3& v) const {
    return Vector3(x - v.x, y - v.y, z - v.z);
}

//multiplication vecteur × scalaire(nombre)
Vector3 Vector3::operator*(float t) const {
    return Vector3(x * t, y * t, z * t);
}

//Get the length with pythagoras
float Vector3::Length() const {
    return std::sqrt(x * x + y * y + z * z);
}

//Normalization of a directional vector (longueur 1)
Vector3 Vector3::Normalized() const {
    float len = Length();
    if (len > 0) {
        return Vector3(x / len, y / len, z / len);
    }
    return Vector3(0, 0, 0);
}

//complement for rayon = o + t*d
//travel t units in the direction of our ray
Vector3 operator*(float t, const Vector3& v) {
    return v * t;
}

//dot product (vector * vector)
float Vector3::operator*(const Vector3& other) const {
    return x * other.x + y * other.y + z * other.z;
}