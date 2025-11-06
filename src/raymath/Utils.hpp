#pragma once

#include <random>
#include <cmath>
#include <limits>

// Returns a random float between 0.0 (inclusive) and 1.0 (exclusive)
inline float randomFloat()
{
    thread_local static std::uniform_real_distribution<float> distribution(0.0f, 1.0f); // thradlocal pour etre safe
    thread_local static std::mt19937 generator;
    return distribution(generator);
}

// Returns a random float in [min, max)

inline float randomFloat(float min, float max)
{
    return min + (max - min) * randomFloat();
}

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385f;

inline float degreesToRadians(float degrees)
{
    return degrees * pi / 180.0f;
}

inline Vector3 randomInUnitSphere()
{
    while (true)
    {
        Vector3 p = {randomFloat(-1, 1), randomFloat(-1, 1), randomFloat(-1, 1)};
        if (p * p < 1.0f)
            return p;
    }
}