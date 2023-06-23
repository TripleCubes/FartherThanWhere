#version 330 core

in vec3 fragment_pos;
in vec3 fragment_normal;
in vec2 fragment_textureCoords;
in float fragment_layerIndex;
layout (location = 0) out vec3 out_pos;
layout (location = 1) out vec3 out_normal;
layout (location = 2) out vec3 out_color;

uniform sampler2DArray texture;

void main() {
    vec4 color = texture(texture, vec3(fragment_textureCoords, fragment_layerIndex));
    if (color.z == 0) {
        discard;
    }
    out_pos = fragment_pos;
    out_normal = fragment_normal;
    out_color = color.xyz;
}
