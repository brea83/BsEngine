#version 330 core
#define MAX_LIGHTS 6


//// lightType enum in Component.h
// enum LightType
//    {
//        Point,       0
//        Directional, 1
//        Spot,        2
//        // END used for imgui combo windows
//        END
//    };
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



out vec4 FragColor;

in vec3 WorldPos;
in vec2 UV;
in vec3 Normal;
in vec3 VertexColor;
//in vec3 ViewVector;

uniform vec3 cameraPosition;
uniform vec4 baseColor = vec4 (1.0, 1.0, 1.0, 1.0);

uniform vec4 ambientLight = vec4(0.1, 0.1, 0.1, 1.0);
uniform int  lightTypes[MAX_LIGHTS];
uniform vec3 lightPosition[MAX_LIGHTS];
uniform vec3 lightDirection[MAX_LIGHTS];
uniform vec3 lightColor[MAX_LIGHTS];

// light attnuation X constant Y linear, Z quadratic
uniform vec3 lightAttenuation[MAX_LIGHTS];
uniform float cutoffAngle[MAX_LIGHTS];

uniform int activeLights;
//uniform DirectionalLightData MainLight;
uniform bool bUseMainLight;

uniform Material material;
uniform sampler2D Texture1;
uniform sampler2D MetallicMap;

vec3 GetPointLuminosity(float nDotL, vec3 lightColor, vec3 attenuation)
{
	return vec3(0,0,0);
}

vec3 GetDirectionalLuminosity(float nDotL, vec3 lightColor)
{
	return  nDotL * lightColor * 0.8;
}

vec3 GetSpotLuminosity(float nDotL, vec3 lightColor, vec3 attenuation, float cutoff)
{
	return vec3(0,0,0);
}

void main()
{
	//FragColor = texture(Texture1, UV);
	FragColor = vec4(0, 0, 0, 1);
	vec3 textureColor = texture(Texture1, UV).rgb;

	//FragColor.xyz += ambientLight.xyz ; // mult in material ambience when I have it
	if(bUseMainLight)
	{
	// set up material stuff that is the same for all light types
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
		FragColor.xyz += ambientLight.xyz;

		vec3 N = normalize(Normal);
		vec3 V = normalize(cameraPosition.xyz - WorldPos);

		vec3 accumulatedDiffuse = vec3(0,0,0);
		vec3 accumulatedSpecular = vec3(0,0,0);

		for(int i = 0; i < activeLights; i++)
		{
			float nDotL = max(dot(N, lightDirection[i]), 0.0);

			if(nDotL > 0)
			{

				if(lightTypes[i] == 0) // is point light
				{
					accumulatedDiffuse += GetPointLuminosity(nDotL, lightColor[i], lightAttenuation[i]);
				}

				if(lightTypes[i] == 1) // is directional light
				{
					accumulatedDiffuse += GetDirectionalLuminosity(nDotL, lightColor[i]);
				}

				if(lightTypes[i] == 2) // is spot light
				{
					accumulatedDiffuse += GetSpotLuminosity(nDotL, lightColor[i], lightAttenuation[i], cutoffAngle[i]);
				}
			}
		}	


//			float distance = length(lightPosition - WorldPos);
//			attenuation = 1.0 / (MainLight.attenuation.x + (MainLight.attenuation.y * distance) + (MainLight.attenuation.z * pow(distance, 2)));
			// until I start doing more pbr like stuff assume the diffuse intensity is just a little less than full reflection
			

		//specular
//			vec3 H = normalize((MainLight.direction + V));
//			float nDotH = max(dot(N, H), 0.0);
//
//			float specBrightness = nDotH * smoothness;
//			specBrightness = pow(max(specBrightness, 0.0), material.specularPower);
//
//			// until I start doing more pbr assume light intensity on spec is about a third of the main light color
//			specular = specBrightness * (MainLight.color * 0.3);
//
//			FragColor.xyz += (diffuse + specular) * textureColor; 
		

	}
	else
	{
		FragColor.xyz = textureColor;
	}

	
		//FragColor.xyz = V;
}