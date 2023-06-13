#version 330 core

layout (location = 0) in vec3 vertex_pos;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

uniform vec3 boxSize;
uniform vec3 boxMargin;

void main() {
    vec3 boxSize2 = boxSize - boxMargin*2;
    gl_Position = projectionMat * viewMat * modelMat * vec4(vertex_pos.x*boxSize2.x + boxMargin.x, 
                                                            vertex_pos.y*boxSize2.y + boxMargin.y, 
                                                            vertex_pos.z*boxSize2.z + boxMargin.z, 1.0);
}
