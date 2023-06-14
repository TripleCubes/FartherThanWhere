#version 330 core

out vec4 out_color;
in vec2 fragment_textureCoords;

uniform sampler2D texture;

void main() {
    out_color = texture(texture, fragment_textureCoords);
}
