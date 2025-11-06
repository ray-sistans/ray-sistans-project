#pragma once

#include <iostream>
#include <algorithm>

class Color
{
private:
  float r = 0;
  float b = 0;
  float g = 0;

public:
  Color();
  Color(float r, float g, float b);
  ~Color();

  float R() const { return r; }
  float G() const { return g; }
  float B() const { return b; }

  Color operator+(Color const &col) const;
  Color operator*(float multiplier) const;
  Color operator*(Color const &col) const;
  Color &operator=(Color const &col);
  Color &operator+=(Color const &col);
  Color operator/(float t) const;

  //  Clamps all color channels to be within [min, max]

  void clamp(float min = 0.0f, float max = 0.999f);

  friend Color operator*(float scalar, const Color &c);
  friend std::ostream &operator<<(std::ostream &_stream, Color const &col);
};
