#pragma once

#include "Color.hpp"

class Material
{
public:
    Color color;
    float reflectivity; // 0 mat, 1 miroir
    float fuzziness;    // 0 net, 1 granuleux

    Material(Color c, float refl, float f)
        : color(c), reflectivity(refl), fuzziness(f) {}

    static Material Matte(Color c)
    {
        return Material(c, 0.0f, 0.0f);
    }

    static Material Mirror(Color c)
    {
        return Material(c, 1.0f, 0.0f);
    }

    static Material PolishedMetal(Color c)
    {
        return Material(c, 0.95f, 0.05f);
    }

    static Material BrushedMetal(Color c)
    {
        return Material(c, 0.9f, 0.2f);
    }

    static Material Gold()
    {
        return Material(Color(1.0f, 0.77f, 0.33f), 0.9f, 0.2f);
    }

    static Material Silver()
    {
        return Material(Color(0.97f, 0.96f, 0.91f), 0.8f, 0.2f);
    }
};