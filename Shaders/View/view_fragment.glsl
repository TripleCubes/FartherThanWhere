#version 330 core

in vec3 fragment_pos;
in vec3 fragment_normal;
in vec2 fragment_textureCoord;
out vec4 out_Color;

vec3 lightDir = vec3(0.4, 1, 0.4);

void main()
{
    vec3 result = vec3(1, 1, 1);

    float light = max(0.2, dot(lightDir, fragment_normal));
    result *= light;

    out_Color = vec4(result, 1);
}
