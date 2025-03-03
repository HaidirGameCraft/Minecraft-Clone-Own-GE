#include <engine/Vector.h>
#include <engine/Mat4.h>
#include <cmath>

Vec3::Vec3()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Vec3::Vec3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vec3 Vec3::Normalize() {
    float length = this->x * this->x + this->y * this->y + this->z * this->z;
    if( length > 0.000001 )
    {
        length = sqrt(length);
        Div( length );

    }

    return *this;
}

Vec3 Vec3::CrossProduct(const Vec3& a, const Vec3& b) {
    Vec3 vec;
    vec.x = a.y * b.z - a.z * b.y;
    vec.y = a.x * b.z - a.z * b.x;
    vec.z = a.x * b.y - a.y * b.x;
    return vec;
}

Vec3 Vec3::operator*(const Mat4& right) {
    Vec3 vec;
    vec.x = right.m[0] * this->x + right.m[4] * this->y + right.m[8] * this->z + right.m[12];
    vec.y = right.m[1] * this->x + right.m[5] * this->y + right.m[9] * this->z + right.m[13];
    vec.z = right.m[2] * this->x + right.m[6] * this->y + right.m[10] * this->z + right.m[14];
    float w = right.m[3] * this->x + right.m[7] * this->y + right.m[12] * this->z + right.m[15];
    if( w > 0.000001 )
        vec.Div(w);
    
    return vec;
}

std::ostream& operator<<(std::ostream& os, const Vec3& vector)
{
    os << "Vec3: [" << vector.x << ", " << vector.y << ", " << vector.z << "]";
    return os;
}

void Vec3::Min(const Vec3 &vec)
{
    this->x = std::min(vec.x, this->x);
    this->y = std::min(vec.y, this->y);
    this->z = std::min(vec.z, this->z);
}
void Vec3::Max(const Vec3& vec) {
    this->x = std::max(vec.x, this->x);
    this->y = std::max(vec.y, this->y);
    this->z = std::max(vec.z, this->z);
}

void Vec3::Add(const Vec3& vec)
{
    this->x += vec.x;
    this->y += vec.y;
    this->z += vec.z;
}

void Vec3::Sub(const Vec3& vec)
{
    this->x -= vec.x;
    this->y -= vec.y;
    this->z -= vec.z;
}

void Vec3::Mult(float scalar)
{
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;
}

void Vec3::Div(float scalar)
{
    this->x *= 1 / scalar;
    this->y *= 1 / scalar;
    this->z *= 1 / scalar;
}

float Vec3::Length() {
    return this->x * this->x + this->y * this->y + this->z * this->z;
}

float Vec3::Dot(const Vec3& point) {
    return this->x * point.x + this->y * point.y + this->z * point.z;
}

Vec3 Vec3::operator+(const Vec3& vec)
{
    Vec3 r(this->x + vec.x, this->y + vec.y, this->z + vec.z);
    return r;
}

Vec3 Vec3::operator-(const Vec3& vec)
{
    Vec3 r(this->x - vec.x, this->y - vec.y, this->z - vec.z);
    return r;
}

Vec3 Vec3::operator*(float scalar)
{
    Vec3 r(this->x * scalar, this->y * scalar, this->z * scalar);
    return r;
}

Vec3 Vec3::operator*(const Vec3& right) {
    Vec3 r(this->x * right.x, this->y * right.y, this->z * right.z);
    return r;
}

Vec3 Vec3::operator/(float scalar)
{
    Vec3 r(this->x / scalar, this->y / scalar, this->z / scalar);
    return r;
}