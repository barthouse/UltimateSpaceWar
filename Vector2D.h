#pragma once

template<typename T>
class Vector2D
{
public:

    Vector2D() : m_x(0), m_y(0) {}
    Vector2D(const T & inX, const T & inY) : m_x(inX), m_y(inY) {}

    Vector2D & Transform(float scale, float rotation, const Vector2D & translation)
    {
        float x = m_x * scale;
        float y = m_y * scale;

        m_x = (T)(x * cos(rotation) - y * sin(rotation));
        m_y = (T)(x * sin(rotation) + y * cos(rotation));

        m_x += translation.m_x;
        m_y += translation.m_y;

        return *this;
    }

    T m_x;
    T m_y;

    Vector2D & operator+=(const Vector2D & a) { m_x += a.m_x; m_y += a.m_y; return *this; }
    Vector2D & operator-=(const Vector2D & a) { m_x -= a.m_x; m_y -= a.m_y; return *this; }
    Vector2D & operator*=(const Vector2D & a) { m_x *= a.m_x; m_y *= a.m_y; return *this; }
    Vector2D & operator/=(const Vector2D & a) { m_x /= a.m_x; m_y /= a.m_y; return *this; }

    Vector2D & operator+=(T a) { m_x += a; m_y += a; return *this; }
    Vector2D & operator-=(T a) { m_x -= a; m_y -= a; return *this; }
    Vector2D & operator*=(T a) { m_x *= a; m_y *= a; return *this; }
    Vector2D & operator/=(T a) { m_x /= a; m_y /= a; return *this; }

};

template<typename T>  Vector2D<T> operator+(Vector2D<T> a, const Vector2D<T> & b) { return a += b; }
template<typename T>  Vector2D<T> operator-(Vector2D<T> a, const Vector2D<T> & b) { return a -= b; }
template<typename T>  Vector2D<T> operator*(Vector2D<T> a, const Vector2D<T> & b) { return a *= b; }
template<typename T>  Vector2D<T> operator/(Vector2D<T> a, const Vector2D<T> & b) { return a /= b; }

template<typename T>  Vector2D<T> operator+(Vector2D<T> a, T b) { return a += b; }
template<typename T>  Vector2D<T> operator-(Vector2D<T> a, T b) { return a -= b; }
template<typename T>  Vector2D<T> operator*(Vector2D<T> a, T b) { return a *= b; }
template<typename T>  Vector2D<T> operator/(Vector2D<T> a, T b) { return a /= b; }

typedef Vector2D<float> Float2D;
typedef Vector2D<int> Int2D;

inline Float2D operator+(const Float2D & a, const Int2D & b) { return Float2D(a.m_x + (float)b.m_x, a.m_y + (float)b.m_y); }
inline Float2D operator-(const Float2D & a, const Int2D & b) { return Float2D(a.m_x - (float)b.m_x, a.m_y - (float)b.m_y); }
inline Float2D operator*(const Float2D & a, const Int2D & b) { return Float2D(a.m_x * (float)b.m_x, a.m_y * (float)b.m_y); }
inline Float2D operator/(const Float2D & a, const Int2D & b) { return Float2D(a.m_x / (float)b.m_x, a.m_y / (float)b.m_y); }
