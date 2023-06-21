#version 330 core

out vec4 out_color;
in vec2 fragment_textureCoords;

uniform sampler2D gBuffer_pos;
uniform sampler2D gBuffer_normal;
uniform int numberOfRandomPoints;
uniform int numberOfRandomRotationVecs;
uniform vec3 randomPoints[20];
uniform vec3 randomRotationVecs[10];

uniform mat4 projectionMat;
uniform mat4 viewMat;

vec2 rotate2D(float x, float y, float rotation) {
    return vec2(x*cos(rotation) - y*sin(rotation), x*sin(rotation) + y*cos(rotation));
}

vec3 rotate3D(float x, float y, float z, float rotateX, float rotateY, float rotateZ) {
    vec3 result = vec3(x, y, z);
    vec2 rotateResultX = rotate2D(result.y, result.z, rotateX);
    result.y = rotateResultX.x;
    result.z = rotateResultX.y;
    vec2 rotateResultY = rotate2D(result.x, result.z, rotateY);
    result.x = rotateResultY.x;
    result.z = rotateResultY.y;
    vec2 rotateResultZ = rotate2D(result.x, result.y, rotateZ);
    result.x = rotateResultZ.x;
    result.y = rotateResultZ.y;

    return result;
}

vec3 rotate3D(vec3 point, vec3 dir) {
    return rotate3D(point.x, point.y, point.z, dir.x, dir.y, dir.z);
}

float random(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    vec3 result = vec3(1, 1, 1);
    vec3 normal = texture(gBuffer_normal, fragment_textureCoords).xyz;
    vec3 position = texture(gBuffer_pos, fragment_textureCoords).xyz;

    if (position == vec3(0.0, 0.0, 0.0)) {
        out_color = vec4(result, 1.0);
        return;
    }

    for (int i = 0; i < numberOfRandomPoints; i++) {
        int rotationVecIndex = abs(int(random(fragment_textureCoords * 100) * 100) % numberOfRandomRotationVecs);
        vec3 randomRotatedPoint = rotate3D(randomPoints[i], randomRotationVecs[rotationVecIndex]) * 0.5;
        if (dot(randomRotatedPoint, normal) <= 0) {
            continue;
        }

        vec3 checkPosition = position + randomRotatedPoint;
        vec4 screenCheckPosition = projectionMat * viewMat * vec4(checkPosition, 1.0);
        screenCheckPosition.xyz /= screenCheckPosition.w;
        vec4 positionTextureCheck = vec4(texture(gBuffer_pos, (screenCheckPosition.xy + vec2(1.0)) / 2).xyz, 1.0);
        if (distance(positionTextureCheck.xyz, checkPosition.xyz) > 0.5) {
            continue;
        }
        positionTextureCheck = projectionMat * viewMat * positionTextureCheck;
        positionTextureCheck.xyz /= positionTextureCheck.w;
        if (positionTextureCheck.z < screenCheckPosition.z) {
            result -= vec3(2.0 / numberOfRandomPoints);
        }
    }

    out_color = vec4(result, 1.0);
}
