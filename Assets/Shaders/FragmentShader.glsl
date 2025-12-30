#version 330 core
#define MAX_LIGHTS 6
// MID GREEN DESATURATED (0.4, 0.5, 0.2, 1)
struct MaterialData
{
	sampler2D ColorTexture;
	sampler2D NormalMap;
	sampler2D MetallicMap;

	vec3 Ao;
	vec3 BaseColor;
	
	bool BUseMetallicMap;
	bool BMapIsRoughness;
	float Smoothness;
	float SpecularPower;
};

struct LightData
{
	int Type;
	vec3 Position;
	vec3 Direction;
	vec3 Color;
	vec3 Attenuation;
	float InnerRadius;
	float OuterRadius;
};

out vec4 FragColor;

in vec3 WorldPos;
in vec2 UV;
in vec3 Normal;
in vec3 VertexColor;
//in vec3 ViewVector;

uniform vec3 CameraPosition;

uniform vec4 AmbientLight = vec4(0.1, 0.1, 0.1, 1.0);


uniform bool BUseLights;
uniform int ActiveLights;
uniform LightData Lights[MAX_LIGHTS];

uniform MaterialData Material;

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

// until I start doing more pbr assume light intensity on spec is about a third of the main light color
vec3 GetPointSpecular(float nDotH, vec3 lightColor, float smoothness, float specularPower, float lightDistance, vec3 attenuationConstants)
{
	float attenuation = 1.0 / (attenuationConstants.x + (attenuationConstants.y * lightDistance) + (attenuationConstants.z * pow(lightDistance, 2)));
	float specBrightness = pow(max(nDotH, 0.0), specularPower * smoothness);

			
	return specBrightness * (lightColor * 0.3) * attenuation;
}

vec3 GetDirectionalSpecular(float nDotH, vec3 lightColor, float smoothness, float specularPower)
{
	float specBrightness = pow(max(nDotH, 0.0), smoothness * specularPower);

	return specBrightness * (lightColor * 0.3);

}

void main()
{
	//FragColor = texture(Texture1, UV);
	FragColor = vec4(0, 0, 0, 1);
	vec3 textureColor = texture(Material.ColorTexture, UV).rgb;

	if (BUseLights)
	{
		float smoothness = 1;
		vec3 diffuse = vec3(0, 0, 0);
		vec3 specular = vec3(0, 0, 0);
		if(Material.BUseMetallicMap)
		{
			float mapSmoothness = 1.0 - texture(Material.MetallicMap, UV).g;
			mapSmoothness = Material.BMapIsRoughness ? 1.0 - mapSmoothness : mapSmoothness;
			smoothness = mapSmoothness * Material.Smoothness;
		}
		else
		{
			smoothness = Material.Smoothness;
		}
		smoothness = clamp(smoothness, 0.1, 1.0);


		// ambient lighting
		FragColor.xyz += AmbientLight.xyz * textureColor;

		// set up for diffuse and specular lighting

		vec3 N = normalize(Normal);
		vec3 V = normalize(CameraPosition.xyz - WorldPos);

		vec3 accumulatedDiffuse = vec3(0,0,0);
		vec3 accumulatedSpecular = vec3(0,0,0);

		// light loop
		for(int i = 0; i < ActiveLights; i++)
		{
			LightData light = Lights[i];

			if(light.Type == 0) // is directional light
			{
				vec3 direction = normalize(-light.Direction);
				float nDotL = max(dot(N, direction), 0.0);
				vec3 H = normalize((-light.Direction + V));
				float nDotH = max(dot(N, H), 0.0);

				accumulatedDiffuse += GetDirectionalLuminosity(nDotL, light.Color);
				accumulatedSpecular += GetDirectionalSpecular(nDotH, light.Color, smoothness, Material.SpecularPower);
				continue;
			}

			vec3 direction = normalize(light.Position - WorldPos);
			float nDotL = max(dot(N, direction), 0.0);

			vec3 H = normalize((direction + V));
			float nDotH = max(dot(N, H), 0.0);
			float lightDistance = length(light.Position - WorldPos);

			if(light.Type == 1) // is point light
			{
				accumulatedDiffuse += GetPointLuminosity(nDotL, light.Color, lightDistance, light.Attenuation);
				accumulatedSpecular += GetPointSpecular(nDotH, light.Color, smoothness, Material.SpecularPower, lightDistance, light.Attenuation);
				continue;
			}

			if(light.Type == 2) // is spot light
			{
				float theta = dot(direction, normalize(-light.Direction));
				float epsilon = light.InnerRadius - light.OuterRadius;
				float intensity = clamp((theta - light.OuterRadius) / epsilon, 0.0, 1.0);

				accumulatedDiffuse +=  intensity * GetPointLuminosity(nDotL, light.Color, lightDistance, light.Attenuation);
				accumulatedSpecular += intensity * GetPointSpecular(nDotH, light.Color, smoothness, Material.SpecularPower, lightDistance, light.Attenuation);
				continue;
			}
			
		}	

		FragColor.xyz += (accumulatedDiffuse + accumulatedSpecular) * textureColor;

	}
	else
	{
		FragColor.xyz = textureColor;
	}
}