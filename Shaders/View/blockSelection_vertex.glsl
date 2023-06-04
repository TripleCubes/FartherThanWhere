#version 330 core

layout (location = 0) in vec3 vertex_pos;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main() {
    gl_Position = projectionMat * viewMat * modelMat * vec4(vertex_pos, 1.0);
}
