#version 330 core

layout (location = 0) in vec2 vertex_pos;

out vec2 fragment_textureCoords;

uniform vec2 rectPos;
uniform vec2 rectSize;
uniform vec2 windowSize;

void main() {
    fragment_textureCoords = vertex_pos;
    
    vec2 resultPos = vec2(rectPos.x/windowSize.x + vertex_pos.x*rectSize.x/windowSize.x,
                            rectPos.y/windowSize.y + vertex_pos.y*rectSize.y/windowSize.y);
    resultPos = (resultPos * 2) - 1;
    gl_Position = vec4(resultPos, 0.0, 1.0);
}
