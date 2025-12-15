#version 330 core

// MID GREEN DESATURATED (0.4, 0.5, 0.2, 1)

out vec4 FragColor;

in vec3 WorldPos;
in vec2 UV;
in vec3 Normal;
in vec3 VertexColor;
in vec3 ViewVector;

uniform vec4 cameraPosition;
uniform vec4 baseColor = vec4 (1.0, 1.0, 1.0, 1.0);

uniform vec4 ambientLight = vec4(0.2, 0.2, 0.2, 1.0);
uniform vec3 lightPosition = vec3(10, 5, -10);
uniform vec3 lightColor = vec3(1.0, 1.0, 0.95);
uniform vec3 lightSpecColor = vec3(1.0, 1.0, 0.95);
// light attnuation X constant Y linear, Z quadratic
uniform vec3 lightAttenuation = vec3(1.0, 0.01, 0.00001);

//TODO add more material uniforms
uniform int shineIntensity = 20;

uniform sampler2D Texture1;

void main()
{
	//FragColor = texture(Texture1, UV);
	vec3 textureColor = texture(Texture1, UV).xyz;
	FragColor = vec4(0, 0, 0, 1);
	//FragColor.xyz += ambientLight.xyz ; // mult in material ambience when I have it

	vec3 lightDirection = normalize(lightPosition - WorldPos);

	vec3 N = normalize(Normal);
	float nDotL = max(dot(N, lightDirection), 0.0);

	if(nDotL > 0)
	{
		vec3 diffuse = nDotL * (lightColor.xyz * textureColor);
		float distance = length(lightPosition - WorldPos);

		float attenuation = 1.0 / (lightAttenuation.x + lightAttenuation.y * distance +lightAttenuation.z * pow(distance, 2));

		//FragColor.xyz += diffuse * attenuation;

	//specular
		vec3 V = normalize(ViewVector);
		vec3 H = normalize((lightDirection + V) / 2);
		float nDotH = max(dot(N, H), 0.0);

		float specBrightness = nDotH;
		for(int i = 0; i < shineIntensity; i++)
		{
			specBrightness *= nDotH;
		}
		vec3 specular = specBrightness * lightSpecColor;

		FragColor.xyz += (ambientLight.xyz + diffuse + specular) * textureColor * attenuation; // multiply in material specular when I have it
	}
		FragColor.w = 1;
}