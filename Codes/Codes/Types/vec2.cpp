#include <Codes/Types/vec2.h>

#include <cmath>
#include <Codes/print.h>

Vec2::Vec2(float x, float y): x(x), y(y) {}

bool Vec2::operator == (Vec2 vec) const {
    return x==vec.x && y==vec.y;
}

Vec2 Vec2::operator + (Vec2 vec) const {
    return Vec2(x+vec.x, y+vec.y);
}

Vec2 Vec2::operator - (Vec2 vec) const {
    return Vec2(x-vec.x, y-vec.y);
}

Vec2 Vec2::operator * (float num) const {
    return Vec2(x*num, y*num);
}

Vec2 Vec2::operator / (float num) const {
    return Vec2(x/num, y/num);
}

float Vec2::length() const {
    return sqrt(x*x + y*y);
}

Vec2 Vec2::normalize() const {
    if (*this == Vec2(0, 0)) {
        PRINTLN("Cant normalize Vec2(0, 0)"); 
        return Vec2(0, 0);
    }

    return *this / this->length();
}

float Vec2::getYFromX(Vec2 vec, float x) const {
    Vec2 differentsVec = vec - Vec2(this->x, this->y);
    if (differentsVec.x == 0)
    {
        PRINTLN("Vec2 getYFromX error: differentsVec.x == 0");
        return this->y;
    }
    if (differentsVec.y == 0)
    {
        return this->y;
    }
    if (x == this->x)
    {
        return this->y;
    }
    if (x == vec.x)
    {
        return vec.y;
    }
    
    float m = (this->y - vec.y)/(this->x - vec.x);
    float a = this->y - m*this->x;
    return m*x + a;
}
