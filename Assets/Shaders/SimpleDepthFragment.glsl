#version 450 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 screenSpacePos;

uniform float Gamma = 10.0;// og srgb is 2.2
void main()
{
	gl_FragDepth = gl_FragCoord.z;
	FragColor = vec4(0.5, 0.5, gl_FragCoord.z, 1);
	FragColor.rgb = clamp(pow(FragColor.rgb, vec3(1.0/Gamma)), 0.0, 1.0);

	screenSpacePos = vec4(gl_FragCoord.xyz, 1);
}