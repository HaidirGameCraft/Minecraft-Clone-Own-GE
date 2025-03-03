#include <engine/Vector.h>

Vec4::Vec4() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->w = 1;
}

Vec4::Vec4(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

void Vec4::Normalize() {
    
}
float Vec4::Length() {
    return this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w;
}
float Vec4::Dot(const Vec4& point) {
    return this->x * point.x + this->y * point.y + this->z * point.z + this->w * point.w;
}
void Vec4::CrossProduct(const Vec4& a, const Vec4& b) {

}
void Vec4::Add(const Vec4& vec) {

}
void Vec4::Sub(const Vec4& vec) {

}
void Vec4::Mult(float scalar) {

}
void Vec4::Div( float scalar ) {

}
void Vec4::Min(const Vec4& vec) {

}
void Vec4::Max(const Vec4& vec) {

}
Vec4 Vec4::operator+(const Vec4& right) {
    Vec4 v;
    v.x = this->x + right.x;
    v.y = this->x + right.y;
    v.z = this->x + right.z;
    v.w = this->x + right.w;
    return v;
}
Vec4 Vec4::operator-(const Vec4& right) {
    Vec4 v;
    v.x = this->x + right.x;
    v.y = this->x + right.y;
    v.z = this->x + right.z;
    v.w = this->x + right.w;
    return v;
}
Vec4 Vec4::operator*(float scalar) {
    Vec4 v;
    v.x = this->x + scalar;
    v.y = this->x + scalar;
    v.z = this->x + scalar;
    v.w = this->x + scalar;
    return v;
}
Vec4 Vec4::operator/(float scalar) {
    Vec4 v;
    v.x = this->x + scalar;
    v.y = this->x + scalar;
    v.z = this->x + scalar;
    v.w = this->x + scalar;
    return v;
}
Vec4 Vec4::operator*(const Mat4& right) {

}

std::ostream& operator<<(std::ostream& os, const Vec4& vector) {

}