#version 330 core
layout(location = 0) out vec3 FragColor;
layout(location = 1) out vec3 screenSpacePos;

void main()
{
	gl_FragDepth = gl_FragCoord.z;
	FragColor = gl_FragCoord.xyz;

	screenSpacePos = gl_FragCoord.xyz;
}