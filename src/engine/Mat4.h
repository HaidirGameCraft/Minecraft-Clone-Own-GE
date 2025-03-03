#pragma once

#include <engine/Vector.h>
#include <iostream>

class Mat4 {
public:
    float m[16];
    Mat4();
    Mat4(const Mat4& mat);
    Mat4(const float mat[]);
    void Identify();
    void Print();

    friend std::ostream& operator<<(std::ostream& os, const Mat4& mat);

    float& operator[](int index);
    Vec3  operator*(const Vec3& right);
    Mat4 operator*(const Mat4& right);

    static void Multiply(const Mat4& a, const Mat4& b, Mat4& out);
    static void Inverse(const Mat4& mat, Mat4& out);
    static float Determinate(const Mat4& mat);

    static void Translate(const Mat4& mat, const Vec3& translate, Mat4 *out);
    static void Scale(const Mat4& mat, const Vec3& scale, Mat4 *out);
    static void Rotation(const Mat4& mat, const Vec3& rotation, Mat4 *out);

    static void RotationX(const Mat4& mat, float x, Mat4 *out);
    static void RotationY(const Mat4& mat, float x, Mat4 *out);
    static void RotationZ(const Mat4& mat, float x, Mat4 *out);
    
};