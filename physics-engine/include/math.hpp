#pragma once

#include <cmath>

struct Vec2
{
    float x;
    float y;

    Vec2() = default;

    Vec2(float x, float y) : x{x}, y{y}
    {}

    [[nodiscard]] float magnitude() const
    {
        return sqrtf(x * x + y * y);
    }

    [[nodiscard]] float sqrMagnitude() const
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