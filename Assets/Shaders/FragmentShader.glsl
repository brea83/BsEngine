#version 330 core

// MID GREEN DESATURATED (0.4, 0.5, 0.2, 1)
struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct LightData
{
	vec3 ambient;
	vec3 position;
	vec3 color;
	vec3 attenuation;
};

out vec4 FragColor;

in vec3 WorldPos;
in vec2 UV;
in vec3 Normal;
in vec3 VertexColor;
//in vec3 ViewVector;

uniform vec4 cameraPosition;
uniform vec4 baseColor = vec4 (1.0, 1.0, 1.0, 1.0);

uniform vec4 ambientLight = vec4(0.1, 0.1, 0.1, 1.0);
uniform vec3 lightPosition = vec3(10, 5, -15);
uniform vec3 lightColor = vec3(1.0, 1.0, 0.95);
uniform vec3 lightSpecColor = vec3(1.0, 1.0, 0.95);
// light attnuation X constant Y linear, Z quadratic
uniform vec3 lightAttenuation = vec3(1.0, 0.01, 0.00001);

//TODO add more material uniforms
uniform int shineIntensity = 80;

uniform Material material;
uniform sampler2D Texture1;
uniform sampler2D SpecularMap;

void main()
{
	//FragColor = texture(Texture1, UV);
	FragColor = vec4(0, 0, 0, 1);
	vec3 textureColor = texture(Texture1, UV).xyz;
	vec3 specMap = texture(SpecularMap, UV).xyz;
	vec3 diffuse = vec3(0, 0, 0);
	vec3 specular = vec3(0, 0, 0);
	//float specStrength = 0.5;
	float attenuation = 1;

	//FragColor.xyz += ambientLight.xyz ; // mult in material ambience when I have it

	vec3 lightDirection = normalize(lightPosition - WorldPos);

	vec3 N = normalize(Normal);
	float nDotL = max(dot(N, lightDirection), 0.0);
	vec3 V = normalize(cameraPosition.xyz - WorldPos);

	if(nDotL > 0)
	{
		float distance = length(lightPosition - WorldPos);
		attenuation = 1.0 / (lightAttenuation.x + lightAttenuation.y * distance +lightAttenuation.z * pow(distance, 2));

		diffuse = nDotL * (lightColor.xyz * textureColor) * attenuation;

		//FragColor.xyz += diffuse * attenuation;

	//specular
		vec3 H = normalize((lightDirection + V)/2);
		float nDotH = max(dot(N, H), 0.0);

		float specBrightness = nDotH * specMap.x;
//		for(int i = 0; i < shineIntensity; i++)
//		{
//			specBrightness *= nDotH;
//		}
		specBrightness = pow(max(specBrightness, 0.0), shineIntensity);
		specular = specBrightness * lightColor * attenuation;

	}
		FragColor.xyz += (ambientLight.xyz + diffuse + specular) * textureColor; 
		//FragColor.xyz = V;
}