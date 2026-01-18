
#version 450 core
layout (triangles, invocations = 5) in;
layout (triangle_strip, max_vertices = 3) out;
//out int gl_Layer;

//uniform mat4 transform;
//uniform mat4 lightViewMat;
//uniform mat4 lightProjMat;

layout (std140, binding = 1) uniform LightProjectionBlock
{
    float farPlane;
    vec4 cascadePlaneDistances;
    //vec4 mainLightPosition; //alignment 0
    mat4 lightSpaceMatrices[16];
};

void main()
{
    for(int i = 0; i < 3; i++)
    {
        gl_Position = lightSpaceMatrices[gl_InvocationID] * gl_in[i].gl_Position;
        gl_Layer = gl_InvocationID;
        EmitVertex();
    }
    EndPrimitive();
}