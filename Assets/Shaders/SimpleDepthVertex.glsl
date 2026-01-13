
#version 330 core

// OpenGL guarantees there are always at least 16 locations
// for 4-component vertex attributes
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in vec2 uvCoord;
layout (location = 4) in vec3 vertexTangent;
layout (location = 5) in vec3 vertexBitangent;

uniform mat4 transform;
//uniform mat4 lightViewMat;
//uniform mat4 lightProjMat;

layout (std140) uniform LightProjectionBlock
{
    vec4 mainLightPosition; //alignment 0
    mat4 lightViewMat;      //         16
    mat4 lightProjMat;      //        80?
};

void main()
{
    gl_Position = lightProjMat * lightViewMat * transform * vec4(vertexPosition, 1.0);
   
}