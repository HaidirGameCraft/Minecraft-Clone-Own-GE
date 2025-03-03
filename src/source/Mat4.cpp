#include <engine/Mat4.h>
#include <cmath>
#include <iostream>

Mat4::Mat4()
{
    for(int i = 0; i < 16; i++)
    {
        this->m[i] = 0;
    }
}

Mat4::Mat4(const Mat4& mat)
{
    for(int i = 0; i < 16; i++)
    {
        this->m[i] = mat.m[i];
    }
}

Mat4::Mat4(const float mat[])
{
    for(int i = 0; i < 16; i++)
    {
        this->m[i] = mat[i];
    }
}

float& Mat4::operator[](int index)
{
    return this->m[index];
}

void Mat4::Identify()
{
    this->m[ 0] = 1;
    this->m[ 1] = 0;
    this->m[ 2] = 0;
    this->m[ 3] = 0;

    this->m[ 4] = 0;
    this->m[ 5] = 1;
    this->m[ 6] = 0;
    this->m[ 7] = 0;

    this->m[ 8] = 0;
    this->m[ 9] = 0;
    this->m[10] = 1;
    this->m[11] = 0;

    this->m[12] = 0;
    this->m[13] = 0;
    this->m[14] = 0;
    this->m[15] = 1;
}

void Mat4::Multiply(const Mat4& a, const Mat4& b, Mat4& out)
{
    Mat4* tmp1 = new Mat4(a);
    Mat4* tmp2 = new Mat4(b);

    for(int i = 0; i < 4; i++)
    {
        out.m[i * 4 + 0] = tmp1->m[i * 4 + 0] * tmp2->m[0] + tmp1->m[i * 4 + 1] * tmp2->m[4] + tmp1->m[i * 4 + 2] * tmp2->m[ 8] + tmp1->m[i * 4 + 3] * tmp2->m[12];
        out.m[i * 4 + 1] = tmp1->m[i * 4 + 0] * tmp2->m[1] + tmp1->m[i * 4 + 1] * tmp2->m[5] + tmp1->m[i * 4 + 2] * tmp2->m[ 9] + tmp1->m[i * 4 + 3] * tmp2->m[13];
        out.m[i * 4 + 2] = tmp1->m[i * 4 + 0] * tmp2->m[2] + tmp1->m[i * 4 + 1] * tmp2->m[6] + tmp1->m[i * 4 + 2] * tmp2->m[10] + tmp1->m[i * 4 + 3] * tmp2->m[14];
        out.m[i * 4 + 3] = tmp1->m[i * 4 + 0] * tmp2->m[3] + tmp1->m[i * 4 + 1] * tmp2->m[7] + tmp1->m[i * 4 + 2] * tmp2->m[11] + tmp1->m[i * 4 + 3] * tmp2->m[15];
    }

    delete(tmp1);
    delete(tmp2);
}

Mat4 Mat4::operator*(const Mat4& right) {
    Mat4 out;
    
    for(int i = 0; i < 4; i++)
    {
        out.m[i * 4 + 0] = this->m[i * 4 + 0] * right.m[0] + this->m[i * 4 + 1] * right.m[4] + this->m[i * 4 + 2] * right.m[ 8] + this->m[i * 4 + 3] * right.m[12];
        out.m[i * 4 + 1] = this->m[i * 4 + 0] * right.m[1] + this->m[i * 4 + 1] * right.m[5] + this->m[i * 4 + 2] * right.m[ 9] + this->m[i * 4 + 3] * right.m[13];
        out.m[i * 4 + 2] = this->m[i * 4 + 0] * right.m[2] + this->m[i * 4 + 1] * right.m[6] + this->m[i * 4 + 2] * right.m[10] + this->m[i * 4 + 3] * right.m[14];
        out.m[i * 4 + 3] = this->m[i * 4 + 0] * right.m[3] + this->m[i * 4 + 1] * right.m[7] + this->m[i * 4 + 2] * right.m[11] + this->m[i * 4 + 3] * right.m[15];
    }

    return out;
}

float Mat4::Determinate(const Mat4& mat)
{
    float left =    (mat.m[0] * mat.m[5] * mat.m[10] * mat.m[15]) + 
                    (mat.m[1] * mat.m[6] * mat.m[11] * mat.m[12]) + 
                    (mat.m[2] * mat.m[7] * mat.m[8] * mat.m[13]) + 
                    (mat.m[3] * mat.m[4] * mat.m[9] * mat.m[14]);

    float right =   (mat.m[3] * mat.m[6] * mat.m[9] * mat.m[12]) + 
                    (mat.m[2] * mat.m[5] * mat.m[8] * mat.m[15]) + 
                    (mat.m[1] * mat.m[4] * mat.m[11] * mat.m[14]) + 
                    (mat.m[0] * mat.m[7] * mat.m[10] * mat.m[13]);
    return left - right;
}

float CrossMat3(const Mat4& mat, int a, int b, int c, 
                                int d, int e, int f, 
                                int g, int h, int i)
{
    float left =    (mat.m[a] * mat.m[e] * mat.m[i]) + 
                    (mat.m[b] * mat.m[f] * mat.m[g]) + 
                    (mat.m[c] * mat.m[d] * mat.m[h]);

    float right =   (mat.m[c] * mat.m[e] * mat.m[g]) + 
                    (mat.m[b] * mat.m[d] * mat.m[i]) +
                    (mat.m[a] * mat.m[f] * mat.m[h]);
    return left - right;
}

void Mat4::Print() {
    for(int i = 0; i < 4; i++)
    {
        std::cout << "| ";
        for(int j = 0; j < 4; j++)
        {
            std::cout << this->m[i * 4 + j] << " ";
        }
        std::cout << "|" << std::endl;
    }
}

std::ostream& operator<<(std::ostream& os, const Mat4& mat) {
    for(int i = 0; i < 4; i++)
    {
        os << "| ";
        for(int j = 0; j < 4; j++)
        {
            std::cout << mat.m[i * 4 + j] << " ";
        }
        os << "|" << std::endl;
    }
    return os;
}

void Mat4::Inverse(const Mat4& mat, Mat4& out)
{
    float det = Mat4::Determinate(mat);
    if( det < 0.000001 )
        det = 0.000001;

    /*
        | 0, 1, 2, 3 |
        | 4, 5, 6, 7 |
        | 8, 9,10,11 |
        |12,13,14,15 |
    */

    float m11 = CrossMat3(mat, 5, 6, 7, 
                                9, 10, 11, 
                                13, 14, 15);

    float m12 = -CrossMat3(mat, 4, 6, 7, 
                                8, 10, 11, 
                                12, 14, 15);

    float m13 = CrossMat3(mat, 4, 5, 7, 
                                8, 9, 11, 
                                12, 13, 15);
    float m14 = -CrossMat3(mat, 4, 5, 6, 
                                8,  9, 10, 
                                12, 13, 14);
    /*
        | 0, 1, 2, 3 |
        | 4, 5, 6, 7 |
        | 8, 9,10,11 |
        |12,13,14,15 |
    */
    float m21 = -CrossMat3(mat, 1, 2, 3, 
                                9, 10, 11, 
                                13, 14, 15);

    float m22 = CrossMat3(mat, 0, 2, 3, 
                                8, 10, 11, 
                                12, 14, 15);

    float m23 = -CrossMat3(mat, 0, 1, 3, 
                                8, 9, 11, 
                                12, 13, 15);

    float m24 = CrossMat3(mat, 0, 1, 2, 
                                8, 9, 10, 
                                12, 13, 14);
    /*
        | 0, 1, 2, 3 |
        | 4, 5, 6, 7 |
        | 8, 9,10,11 |
        |12,13,14,15 |
    */
    float m31 = CrossMat3(mat, 1, 2, 3, 
                                5, 6, 7, 
                                13, 14, 15);

    float m32 = -CrossMat3(mat, 0, 2, 3, 
                                4, 6, 7, 
                                12, 14, 15);

    float m33 = CrossMat3(mat, 0, 1, 3, 
                                4, 5, 7, 
                                12, 13, 15);

    float m34 = -CrossMat3(mat, 0, 1, 2, 
                                4, 5, 6, 
                                12, 13, 14);

    /*
        | 0, 1, 2, 3 |
        | 4, 5, 6, 7 |
        | 8, 9,10,11 |
        |12,13,14,15 |
    */
    float m41 = -CrossMat3(mat, 1, 2, 3, 
                                5, 6, 7, 
                                9, 10, 11);

    float m42 = CrossMat3(mat, 0, 2, 3, 
                                4, 6, 7, 
                                8, 10, 11);

    float m43 = -CrossMat3(mat, 0, 1, 3, 
                                4, 5, 7, 
                                8, 9, 11);

    float m44 = CrossMat3(mat, 0, 1, 2, 
                                4, 5, 6, 
                                8, 9, 10);

    out.m[0] = m11;
    out.m[1] = m21;
    out.m[2] = m31;
    out.m[3] = m41;

    out.m[4] = m12;
    out.m[5] = m22;
    out.m[6] = m32;
    out.m[7] = m42;

    out.m[8] = m13;
    out.m[9] = m23;
    out.m[10] = m33;
    out.m[11] = m43;

    out.m[12] = m14;
    out.m[13] = m24;
    out.m[14] = m34;
    out.m[15] = m44;

    for(int i = 0; i < 16; i++)
    {
        out.m[i] *= 1 / det;
    }
}

void Mat4::Translate(const Mat4& mat, const Vec3& translate, Mat4 *out) {
    Mat4 matrix;
    matrix.Identify();
    matrix[12] = translate.x;
    matrix[13] = translate.y;
    matrix[14] = translate.z;

    Mat4::Multiply(mat, matrix, *out);
}
void Mat4::Scale(const Mat4& mat, const Vec3& scale, Mat4 *out) {
    Mat4 matrix;
    matrix.Identify();
    matrix[0] = scale.x;
    matrix[5] = scale.y;
    matrix[10] = scale.z;

    Mat4::Multiply(mat, matrix, *out);
}
void Mat4::Rotation(const Mat4& mat, const Vec3& rotation, Mat4 *out) {
    RotationY(mat, rotation.y, out);
    RotationX(mat, rotation.x, out);
    RotationZ(mat, rotation.z, out);
}

void Mat4::RotationX(const Mat4 &mat, float x, Mat4 *out)
{
    Mat4 *tmp = new Mat4();
    tmp->Identify();
    tmp->m[5] = cosf(x);
    tmp->m[6] = -sinf(x);
    tmp->m[9] = sinf(x);
    tmp->m[10] = cosf(x);

    Mat4::Multiply(*tmp, mat, *out);
    delete(tmp);
}

void Mat4::RotationY(const Mat4 &mat, float x, Mat4 *out)
{
    Mat4 *tmp = new Mat4();
    tmp->Identify();
    tmp->m[0] = cosf(x);
    tmp->m[2] = -sinf(x);
    tmp->m[8] = sinf(x);
    tmp->m[10] = cosf(x);

    Mat4::Multiply(*tmp, mat, *out);
    delete(tmp);
}

void Mat4::RotationZ(const Mat4 &mat, float x, Mat4 *out)
{
    Mat4 *tmp = new Mat4();
    tmp->Identify();
    tmp->m[0] = cosf(x);
    tmp->m[1] = -sinf(x);
    tmp->m[4] = sinf(x);
    tmp->m[5] = cosf(x);

    Mat4::Multiply(*tmp, mat, *out);
    delete(tmp);
}

Vec3 Mat4::operator*(const Vec3& right) {
    
    Vec3 vec;
    vec.x = this->m[0] * right.x + this->m[1] * right.y + this->m[2] * right.z + this->m[3];
    vec.y = this->m[4] * right.x + this->m[5] * right.y + this->m[6] * right.z + this->m[7];
    vec.z = this->m[8] * right.x + this->m[9] * right.y + this->m[10] * right.z + this->m[11];
    float w = this->m[12] * right.x + this->m[13] * right.y + this->m[14] * right.z + this->m[15];
    if( w > 0.000001 )
        vec.Div(w);
    
    return vec;
}