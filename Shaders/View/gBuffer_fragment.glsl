#version 330 core

in vec3 fragment_pos;
in vec3 fragment_normal;
in vec2 fragment_textureCoords;
layout (location = 0) out vec3 out_pos;
layout (location = 1) out vec3 out_normal;
layout (location = 2) out vec3 out_color;

uniform sampler2D texture;

void main() {
    out_pos = fragment_pos;
    out_normal = fragment_normal;
    //out_color = texture(texture, fragment_textureCoords).xyz;
    out_color = vec3(1, 1, 1);
}
