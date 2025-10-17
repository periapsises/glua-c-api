#pragma once

struct Vector
{
    Vector(): x(0f), y(0f), z(0f) {}
    Vector(const Vector& source): x(source.x), y(source.y), z(source.z) {}

    Vector& operator=(const Vector& source)
    {
        x = source.x;
        y = source.y;
        z = source.z;
        return *this;
    }

    float x, y, z;
};

using QAngle = Vector;
