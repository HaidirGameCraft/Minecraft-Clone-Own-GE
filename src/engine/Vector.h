#pragma once
#include <iostream>
struct Mat4;

class Vec3
{
public:
    float x;
    float y;
    float z;
    Vec3();
    Vec3(float x, float y, float z);
    Vec3 Normalize();
    float Length();
    float Dot(const Vec3& point);
    static Vec3 CrossProduct(const Vec3& a, const Vec3& b);
    void Add(const Vec3& vec);
    void Sub(const Vec3& vec);
    void Mult(float scalar);
    void Div( float scalar );

    void Min(const Vec3& vec);
    void Max(const Vec3& vec);

    Vec3 operator+(const Vec3& right);
    Vec3 operator-(const Vec3& right);
    Vec3 operator*(float scalar);
    Vec3 operator*(const Vec3& right);
    Vec3 operator/(float scalar);
    Vec3 operator*(const Mat4& right);

    friend std::ostream& operator<<(std::ostream& os, const Vec3& vector);
};

class Vec4
{
public:
    float x;
    float y;
    float z;
    float w;
    Vec4();
    Vec4(float x, float y, float z, float w = 1);
    void Normalize();
    float Length();
    float Dot(const Vec4& point);
    void CrossProduct(const Vec4& a, const Vec4& b);
    void Add(const Vec4& vec);
    void Sub(const Vec4& vec);
    void Mult(float scalar);
    void Div( float scalar );

    void Min(const Vec4& vec);
    void Max(const Vec4& vec);

    Vec4 operator+(const Vec4& right);
    Vec4 operator-(const Vec4& right);
    Vec4 operator*(float scalar);
    Vec4 operator/(float scalar);
    Vec4 operator*(const Mat4& right);

    friend std::ostream& operator<<(std::ostream& os, const Vec4& vector);
};

class Color {
public:
    float r, g, b, a;
    Color();
    Color(float r, float g, float b, float a);
    void Add(const Color& vec);
    void Sub(const Color& vec);
    void Mult(float scalar);
    void Div( float scalar );

    Color operator+(const Color& right);
    Color operator-(const Color& right);
    Color operator*(float scalar);
    Color operator/(float scalar);
};