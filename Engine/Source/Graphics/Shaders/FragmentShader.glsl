#version 330 core

// MID GREEN DESATURATED (0.4, 0.5, 0.2, 1)

out vec4 FragColor;

in vec2 UV1;
in vec3 Normal;
in vec3 VertexColor;

uniform sampler2D Texture1;

void main()
{
	FragColor = texture(Texture1, UV1);
	//FragColor *= vec4(normalize(Normal), 1.0f);
	//FragColor = vec4(VertexColor, 1.0f);
	//FragColor = vec4(VertexColor, 1.0f);
	//vec3 newNormal = Normal + vec3(0.2f);
	//FragColor = vec4(normalize(newNormal), 1.0f);
}