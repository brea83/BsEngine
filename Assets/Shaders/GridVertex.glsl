
#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in vec2 uvCoord;
layout (location = 4) in vec3 vertexTangent;
layout (location = 5) in vec3 vertexBitangent;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPosition;

uniform int MajorGridDivisions = 10; // suggested range 2 to 25, default 10


out VS_OUT
{
   vec3 Pos_WS;
   vec3 Pos_CS;
   vec4 UV;
   int MajorGridDiv;

} OUT;

void main()
{
    gl_Position = projection * view * transform * vec4(vertexPosition, 1.0);

    OUT.Pos_WS =  vec3(transform * vec4(vertexPosition, 1.0));
    OUT.Pos_CS = vec3(view * transform * vec4(vertexPosition, 1.0));
   

   float div = max(2.0, round(MajorGridDivisions));

   // this should reduce visual artifacts when far from world origin

   vec3 cameraCenteringOffset = floor(cameraPosition / div) * div;

   OUT.UV.yx = (OUT.Pos_WS - cameraCenteringOffset).yz;
   OUT.UV.wz = OUT.Pos_WS.yz;

   OUT.MajorGridDiv = MajorGridDivisions;
}