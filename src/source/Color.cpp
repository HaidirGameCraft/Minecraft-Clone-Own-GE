#include <engine/Vector.h>

Color::Color()
{
    this->r = 0;
    this->g = 0;
    this->b = 0;
    this->a = 0;
}

Color::Color(float r, float g, float b, float a)
{
    this->r = a;
    this->g = g;
    this->b = b;
    this->a = a;
}

void Color::Add(const Color& vec) {
    this->r += vec.r;
    this->g += vec.g;
    this->b += vec.b;
    this->a += vec.a;
}
void Color::Sub(const Color& vec) {
    this->r -= vec.r;
    this->g -= vec.g;
    this->b -= vec.b;
    this->a -= vec.a;
}
void Color::Mult(float scalar) {

}
void Color::Div( float scalar ) {

}

Color Color::operator+(const Color& right) {

}
Color Color::operator-(const Color& right) {

}
Color Color::operator*(float scalar) {

}
Color Color::operator/(float scalar) {

}