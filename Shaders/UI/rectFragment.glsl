#version 330 core

out vec4 out_color;
in vec2 fragment_textureCoords;

uniform bool drawTexture;
uniform vec4 rectColor;
uniform sampler2D rectTexture;

void main()
{
    if (!drawTexture) {
        out_color = rectColor;
        return;
    }

    out_color = texture(rectTexture, fragment_textureCoords);
}
