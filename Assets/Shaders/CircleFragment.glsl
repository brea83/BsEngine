#version 450 core

layout(location = 0) out vec4 FragColor;

uniform float Fade;
uniform float LineWidth;
uniform vec4 Color;

in VS_OUT
{
   vec3 Pos_OS; //Object Space
   vec3 Pos_WS;
   vec3 Pos_CS;
   //vec2 UV;
} IN;

void main()
{
	// Calculate distance and fill circle with white
    float distance = 1.0 - length(IN.Pos_OS);
    float circle = smoothstep(0.0, Fade, distance);
    circle *= smoothstep(LineWidth + Fade, LineWidth, distance);

	if (circle == 0.0)
		discard;

    // Set output color
    FragColor = Color;
	FragColor.a *= circle;

}