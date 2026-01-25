#version 450 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 screenSpacePos;
layout(location = 2) out vec4 test;

uniform float Gamma = 10.0;// og srgb is 2.2
void main()
{
	gl_FragDepth = gl_FragCoord.z;
	FragColor = vec4(0.5, 1.0, 0.5, 1.0);
	//FragColor.rgb = clamp(pow(FragColor.rgb, vec3(1.0/Gamma)), 0.0, 1.0);

	screenSpacePos = vec4( 1.0, 1.0, 1.0, 1.0);
	test = vec4(0.5, 0.5, 1.0, 1.0); 
}