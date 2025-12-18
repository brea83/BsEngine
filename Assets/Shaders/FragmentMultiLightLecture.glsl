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
uniform float innerRadius[MAX_LIGHTS];
uniform float outerRadius[MAX_LIGHTS];

uniform int activeLights;
//uniform DirectionalLightData MainLight;
uniform bool bUseMainLight;

uniform Material material;
uniform sampler2D Texture1;
uniform sampler2D MetallicMap;

// until I start doing more pbr like stuff assume the diffuse intensity is just a little less than full reflection
vec3 GetPointLuminosity(float nDotL, vec3 lightColor, float lightDistance, vec3 attenuationConstants)
{
	float attenuation = 1.0 / (attenuationConstants.x + (attenuationConstants.y * lightDistance) + (attenuationConstants.z * pow(lightDistance, 2)));
	return  nDotL * lightColor * attenuation * 0.8;
}

vec3 GetDirectionalLuminosity(float nDotL, vec3 lightColor)
{
	return  nDotL * lightColor * 0.8;
}

vec3 GetSpotLuminosity(float nDotL, vec3 lightColor, vec3 attenuation, float cutoff)
{
	
	return vec3(0,0,0);
}

// until I start doing more pbr assume light intensity on spec is about a third of the main light color
vec3 GetPointSpecular(float nDotH, vec3 lightColor, float smoothness, float specularPower, float lightDistance, vec3 attenuationConstants)
{
	float attenuation = 1.0 / (attenuationConstants.x + (attenuationConstants.y * lightDistance) + (attenuationConstants.z * pow(lightDistance, 2)));
	float specBrightness = nDotH * smoothness;
	specBrightness = pow(max(specBrightness, 0.0), specularPower);

			
	return specBrightness * (lightColor * 0.3) * attenuation;
}

vec3 GetDirectionalSpecular(float nDotH, vec3 lightColor, float smoothness, float specularPower)
{
	float specBrightness = nDotH * smoothness;
	specBrightness = pow(max(specBrightness, 0.0), specularPower);

			
	return specBrightness * (lightColor * 0.3);

}

vec3 GetSpotSpecular(float nDotH, vec3 lightColor, float smoothness, float specularPower, float lightDistance, vec3 attenuation, float cutoff)
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
			if(lightTypes[i] == 0) // is directional light
			{
				vec3 direction = normalize(-lightDirection[i]);
				float nDotL = max(dot(N, direction), 0.0);
				vec3 H = normalize((-lightDirection[i] + V));
				float nDotH = max(dot(N, H), 0.0);

				accumulatedDiffuse += GetDirectionalLuminosity(nDotL, lightColor[i]);
				accumulatedSpecular += GetDirectionalSpecular(nDotH, lightColor[i], material.smoothness, material.specularPower);
				continue;
			}

			vec3 direction = normalize(lightPosition[i] - WorldPos);
			float nDotL = max(dot(N, direction), 0.0);

			vec3 H = normalize((direction + V));
			float nDotH = max(dot(N, H), 0.0);
			float lightDistance = length(lightPosition[i] - WorldPos);

			if(lightTypes[i] == 1) // is point light
			{
				accumulatedDiffuse += GetPointLuminosity(nDotL, lightColor[i], lightDistance, lightAttenuation[i]);
				accumulatedSpecular += GetPointSpecular(nDotH, lightColor[i], material.smoothness, material.specularPower, lightDistance, lightAttenuation[i]);
				continue;
			}

			if(lightTypes[i] == 2) // is spot light
			{
				float theta = dot(direction, normalize(-lightDirection[i]));
				float epsilon = innerRadius[i] - outerRadius[i];
				float intensity = clamp((theta - outerRadius[i]) / epsilon, 0.0, 1.0);

				accumulatedDiffuse +=  intensity * GetPointLuminosity(nDotL, lightColor[i], lightDistance, lightAttenuation[i]);
				accumulatedSpecular += intensity * GetPointSpecular(nDotH, lightColor[i], material.smoothness, material.specularPower, lightDistance, lightAttenuation[i]);
				continue;
			}
			
		}	
		FragColor.xyz += (accumulatedDiffuse + accumulatedSpecular) * textureColor;

//			
			

		//specular
			
//
			
//
//			FragColor.xyz += (diffuse + specular) * textureColor; 
		

	}
	else
	{
		FragColor.xyz = textureColor;
	}

	
		//FragColor.xyz = V;
}