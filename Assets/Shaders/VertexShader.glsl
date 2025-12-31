
#version 330 core
#define MAX_LIGHTS 6

// OpenGL guarantees there are always at least 16 locations
// for 4-component vertex attributes
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

uniform int activeLightsCount;


uniform sampler2D NormalMap;
uniform bool BUseNormalMap;

out VS_OUT
{
   vec3 Pos_WS;
   vec3 Pos_TS;
   vec3 Pos_CS;
   vec3 Normal_WS;
   vec3 Normal_CS;
   vec2 UV;
   vec3 VertexColor;
   vec3 CameraPos_WS;
   vec3 CameraPos_TS; 
   vec3 EyeDirection_CS;
   mat3 TBN;
} OUT;

void main()
{
    gl_Position = projection * view * transform * vec4(vertexPosition, 1.0);
    OUT.Pos_WS =  vec3(transform * vec4(vertexPosition, 1.0));
    OUT.Pos_CS = vec3(view * transform * vec4(vertexPosition, 1.0));

    mat3 MV3 = mat3(view * transform);
    mat3 M = mat3(transform);
    vec3 t = M * normalize(vertexTangent);
    vec3 b = M * normalize(vertexBitangent);
    vec3 n = M * normalize(vertexNormal);

    OUT.TBN = transpose(mat3(t, b, n));

    OUT.Pos_TS = OUT.TBN * OUT.Pos_WS;

    OUT.CameraPos_WS = cameraPosition;
    OUT.CameraPos_TS = OUT.TBN * cameraPosition;
    OUT.EyeDirection_CS = vec3(0,0,0) - OUT.Pos_CS;



    OUT.Normal_WS = (transform * vec4(vertexNormal, 0.0)).xyz;
    OUT.Normal_CS = (view * transform * vec4(vertexNormal, 0.0)).xyz;

    OUT.UV = uvCoord;

    OUT.VertexColor = vertexColor;

    
}