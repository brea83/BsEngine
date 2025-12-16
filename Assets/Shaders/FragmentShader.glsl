#version 330 core

// MID GREEN DESATURATED (0.4, 0.5, 0.2, 1)
struct Material
{
	vec3 ao;
	vec3 diffuse;
	vec3 specular;
	bool bUseMetalicMap;
	float smoothness;
	float specularPower;
};

struct LightData
{
	vec3 position;
	vec3 color;
	vec3 attenuation;
};

struct DirectionalLightData
{
	vec3 direction;
	vec3 color;
	vec3 attenuation;
};

out vec4 FragColor;

in vec3 WorldPos;
in vec2 UV;
in vec3 Normal;
in vec3 VertexColor;
//in vec3 ViewVector;

uniform vec3 cameraPosition;
uniform vec4 baseColor = vec4 (1.0, 1.0, 1.0, 1.0);

uniform vec4 ambientLight = vec4(0.1, 0.1, 0.1, 1.0);
uniform vec3 lightPosition = vec3(10, 5, -15);
uniform vec3 lightColor = vec3(1.0, 1.0, 0.95);
uniform vec3 lightSpecColor = vec3(1.0, 1.0, 0.95);
// light attnuation X constant Y linear, Z quadratic
uniform vec3 lightAttenuation = vec3(1.0, 0.01, 0.00001);

uniform DirectionalLightData MainLight;
uniform bool bUseMainLight;

uniform Material material;
uniform sampler2D Texture1;
uniform sampler2D MetallicMap;

void main()
{
	//FragColor = texture(Texture1, UV);
	FragColor = vec4(0, 0, 0, 1);
	vec3 textureColor = texture(Texture1, UV).rgb;
	float smoothness = 1;
	vec3 diffuse = vec3(0, 0, 0);
	vec3 specular = vec3(0, 0, 0);
	if(material.bUseMetalicMap)
	{
		smoothness = texture(MetallicMap, UV).g * material.smoothness;
	}
	else
	{
		smoothness = material.smoothness;
	}
	//float specStrength = 0.5;
	float attenuation = 1;

	//FragColor.xyz += ambientLight.xyz ; // mult in material ambience when I have it
	if(bUseMainLight)
	{
		
		FragColor.xyz += ambientLight.xyz * textureColor;
		vec3 N = normalize(Normal);
		float nDotL = max(dot(N, MainLight.direction), 0.0);
		vec3 V = normalize(cameraPosition.xyz - WorldPos);

		if(nDotL > 0)
		{
//			float distance = length(lightPosition - WorldPos);
//			attenuation = 1.0 / (MainLight.attenuation.x + (MainLight.attenuation.y * distance) + (MainLight.attenuation.z * pow(distance, 2)));
			// until I start doing more pbr like stuff assume the diffuse intensity is just a little less than full reflection
			diffuse = nDotL * MainLight.color * 0.8;

		//specular
			vec3 H = normalize((MainLight.direction + V));
			float nDotH = max(dot(N, H), 0.0);

			float specBrightness = nDotH * smoothness;
			specBrightness = pow(max(specBrightness, 0.0), material.specularPower);

			// until I start doing more pbr assume light intensity on spec is about a third of the main light color
			specular = specBrightness * (MainLight.color * 0.3);

			FragColor.xyz += (diffuse + specular) * textureColor; 
		}

	}
	else
	{
		FragColor.xyz = textureColor;
	}

	
		//FragColor.xyz = V;
}