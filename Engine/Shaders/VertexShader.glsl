
#version 330 core

// OpenGL guarantees there are always at least 16 locations
// for 4-component vertex attributes
layout (location = 0) in vec3 positionData;
layout (location = 1) in vec3 colorData;
layout (location = 2) in vec3 normalData;
layout (location = 3) in vec2 uv1;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

out vec2 UV1;
out vec3 Normal;
out vec3 VertexColor;

void main()
{
    gl_Position = projection * view * transform * vec4(positionData, 1.0);
    VertexColor = colorData;
    UV1 = uv1;
}