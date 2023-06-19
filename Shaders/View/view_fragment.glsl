#version 330 core

out vec4 out_color;
in vec2 fragment_textureCoords;

uniform sampler2D gBuffer_pos;
uniform sampler2D gBuffer_normal;
uniform sampler2D gBuffer_color;

vec3 lightDir = vec3(0.4, 1, 0.4);

void main() {
    vec3 pos = texture(gBuffer_pos, fragment_textureCoords).xyz;
    vec3 normal = texture(gBuffer_normal, fragment_textureCoords).xyz;
    vec3 color = texture(gBuffer_color, fragment_textureCoords).xyz;
    
    float light = max(0.2, dot(lightDir, normal));
    vec3 result = light * color;

    out_color = vec4(result, 1);
}
