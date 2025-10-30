#version 330 core

// MID GREEN DESATURATED (0.4, 0.5, 0.2, 1)

out vec4 FragColor;
in vec3 vertexColor;

void main()
{
   FragColor = vec4(vertexColor, 1);
}