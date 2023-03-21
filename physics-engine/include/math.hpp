#pragma once

#include <cmath>

struct Vec2
{
    float x;
    float y;

    float magnitude() const
    {
        return std::sqrt(x * x + y * y);
    }

    float sqrMagnitude() const
    {
        return x * x + y * y;
    }

    void normalize()
    {
        float m = magnitude();
        x /= m;
        y /= m;
    }

    Vec2& operator+=(const Vec2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vec2& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vec2& operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }
};

inline Vec2 operator+(Vec2 a, const Vec2& b)
{
    a += b;
    return a;
}

inline Vec2 operator-(Vec2 a, const Vec2& b)
{
    a -= b;
    return a;
}

inline Vec2 operator*(Vec2 a, float scalar)
{
    a *= scalar;
    return a;
}

inline Vec2 operator/(Vec2 a, float scalar)
{
    a /= scalar;
    return a;
}