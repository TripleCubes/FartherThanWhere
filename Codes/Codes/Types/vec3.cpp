#include <Codes/Types/vec3.h>

#include <Codes/Debug/print.h>
#include <Codes/Types/intPos.h>
#include <cmath>
#include <glm/gtx/rotate_vector.hpp>

Vec3::Vec3(float x, float y, float z): x(x), y(y), z(z) {}

Vec3::Vec3(glm::vec3 glmVec): x(glmVec.x), y(glmVec.y), z(glmVec.z) {}

Vec3::Vec3(IntPos pos): x(pos.x), y(pos.y), z(pos.z) {}

Vec3::Vec3() {}

glm::vec3 Vec3::toGlmVec3() const {
    return glm::vec3(x, y, z);
}

bool Vec3::operator == (Vec3 vec) const {
    return x==vec.x && y==vec.y && z==vec.z;
}

bool Vec3::operator != (Vec3 vec) const {
    return x!=vec.x || y!=vec.y || z!=vec.z;
}

Vec3 Vec3::operator + (Vec3 vec) const {
    return Vec3(x+vec.x, y+vec.y, z+vec.z);
}

Vec3 Vec3::operator - (Vec3 vec) const {
    return Vec3(x-vec.x, y-vec.y, z-vec.z);
}

Vec3 Vec3::operator * (float num) const {
    return Vec3(x*num, y*num, z*num);
}

Vec3 Vec3::operator / (float num) const {
    return Vec3(x/num, y/num, z/num);
}

void Vec3::operator += (Vec3 vec) {
    x += vec.x;
    y += vec.y;
    z += vec.z;
}

void Vec3::operator -= (Vec3 vec) {
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
}

void Vec3::operator *= (float num) {
    x *= num;
    y *= num;
    z *= num;
}

void Vec3::operator /= (float num) {
    x /= num;
    y /= num;
    z /= num;
}

float Vec3::dot(Vec3 vec) const {
    return x*vec.x + y*vec.y + z*vec.z;
}

Vec3 Vec3::cross(Vec3 vec) const {
    return Vec3(glm::cross(this->toGlmVec3(), vec.toGlmVec3()));
}

float Vec3::length() const {
    return sqrt(x*x + y*y + z*z);
}

float Vec3::distance(Vec3 vec) const {
    return sqrt(pow(vec.x-x,2) + pow(vec.y-y,2) + pow(vec.z-z,2));
}

Vec3 Vec3::normalize() const {
    if (*this == Vec3(0, 0, 0)) {
        PRINTLN("Cant normalize Vec3(0, 0, 0)"); 
        return Vec3(0, 0, 0);
    }

    return *this / this->length();
}

Vec3 Vec3::rotateX(float deg) const {
    glm::vec3 glmVec = glm::vec3(x, y, z);
    glmVec = glm::rotateX(glmVec, glm::radians(deg));
    return Vec3(glmVec);
}

Vec3 Vec3::rotateY(float deg) const {
    glm::vec3 glmVec = glm::vec3(x, y, z);
    glmVec = glm::rotateY(glmVec, glm::radians(deg));
    return Vec3(glmVec);
}

Vec3 Vec3::rotateXY(float degX, float degY) const {
    glm::vec3 glmVec = glm::vec3(x, y, z);
    glmVec = glm::rotateX(glmVec, glm::radians(degX));
    glmVec = glm::rotateY(glmVec, glm::radians(degY));
    return Vec3(glmVec);
}
