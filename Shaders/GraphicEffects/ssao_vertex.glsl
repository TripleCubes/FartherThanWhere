#version 330 core

layout (location = 0) in vec2 vertex_pos;

out vec2 fragment_textureCoords;

void main() {
    gl_Position = vec4(vertex_pos, 0.0, 1.0);
    fragment_textureCoords = (vertex_pos + 1) / 2;
}
