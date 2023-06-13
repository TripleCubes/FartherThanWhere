#version 330 core

out vec4 out_color;

uniform vec4 frameColor;

void main() {
    out_color = frameColor;
}
