#version 330 core
layout(location = 1) out vec3 screenSpacePos;
out vec4 FragColor;

void main()
{
	gl_FragDepth = gl_FragCoord.z;
	FragColor = vec4(gl_FragCoord.xyz, 1);

	screenSpacePos = gl_FragCoord.xyz;
}