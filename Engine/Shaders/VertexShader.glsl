
#version 330 core

// OpenGL guarantees there are always at least 16 locations
// for 4-component vertex attributes
layout (location = 0) in vec3 positionData;
layout (location = 1) in vec3 colorData;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertexColor;
void main()
{
    gl_Position = projection * view * transform * vec4(positionData, 1.0);
    vertexColor = colorData;
}