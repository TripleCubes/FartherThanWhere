#ifndef VEC3_H
#define VEC3_H

#include <glm/glm.hpp>
class IntPos;

struct Vec3 {
    float x = 0;
    float y = 0;
    float z = 0;
    Vec3(float x, float y, float z);
    Vec3(glm::vec3 glmVec);
    Vec3(IntPos pos);
    glm::vec3 toGlmVec3() const;

    bool operator == (Vec3 vec) const;
    bool operator != (Vec3 vec) const;
    Vec3 operator + (Vec3 vec) const;
    Vec3 operator - (Vec3 vec) const;
    Vec3 operator * (float num) const;
    Vec3 operator / (float num) const;
    void operator += (Vec3 vec);
    void operator -= (Vec3 vec);
    void operator *= (float num);
    void operator /= (float num);
    float dot(Vec3 vec) const;
    Vec3 cross(Vec3 vec) const;
    float length() const;
    float distance(Vec3 vec) const;
    Vec3 normalize() const;
    Vec3 rotateX(float deg) const;
    Vec3 rotateY(float deg) const;
    Vec3 rotateXY(float degX, float degY) const;
};

#endif
