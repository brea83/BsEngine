
#version 330 core

// OpenGL guarantees there are always at least 16 locations
// for 4-component vertex attributes
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in vec2 uvCoord;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;
//uniform vec4 cameraPosition;

out vec3 WorldPos;
out vec3 Normal;
out vec2 UV;
out vec3 VertexColor;
out vec3 ViewVector;

void main()
{
    gl_Position = projection * view * transform * vec4(vertexPosition, 1.0);
    WorldPos =  vec3(transform * vec4(vertexPosition, 1.0));

    Normal = mat3(transpose(inverse(transform))) * vertexNormal;//vertexNormal;//normalize(vec3(transform * vec4(vertexNormal, 0.0)));
    UV = uvCoord;

    VertexColor = vertexColor;

    
}