#include <cmath>
#include <algorithm> 
#include "Color.hpp"

Color:: Color() : r(0), b(0), g(0)
{  
}

Color:: Color(float iR, float iG, float iB) : r(iR), g(iG), b(iB)
{  
}

Color::~ Color()
{
}

Color Color::operator+(Color const& col) const {
    return Color(r + col.r, g + col.g, b + col.b);
}

Color Color::operator*(float multiplier) const {
    return Color(r * multiplier, g * multiplier, b * multiplier);
}

Color& Color::operator=(Color const& col) {
  r = col.r;
  g = col.g;
  b = col.b;
  return *this;
}

Color operator*(float scalar, const Color& color) {
    return color * scalar; 
}

std::ostream & operator<<(std::ostream & _stream, Color const & col) {  
  return _stream << "(" << col.r << "," << col.g << "," << col.b << ")";
}

Color& Color::operator+=(Color const& col) {
    r += col.r;
    g += col.g;
    b += col.b;
    return *this;
}

// Averages color (pixelColor / 100)
 
Color Color::operator/(float t) const {
    return Color(r / t, g / t, b / t);
}

// On appelle cette fonction UNE SEULE FOIS à la fin.
 
void Color::clamp(float min, float max) {
    r = std::clamp(r, min, max);
    g = std::clamp(g, min, max);
    b = std::clamp(b, min, max);
}
