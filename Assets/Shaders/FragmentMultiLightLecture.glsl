#version 330 core
#define MAX_LIGHTS 6


//// lightType enum in Component.h
// enum LightType
//    {
//        Directional, 0
//        Point,       1
//        Spot,        2
//        // END used for imgui combo windows
//        END
//    };
// MID GREEN DESATURATED (0.4, 0.5, 0.2, 1)
struct MaterialData
{
	sampler2D ColorTexture;
	vec3 BaseColor;

	sampler2D NormalMap;
	bool BUseNormalMap;

	sampler2D MetallicMap;
	bool BUseMetallicMap;
	bool BMapIsRoughness;

	vec3 Ao;
	
	float Smoothness;
	float SpecularPower;
};


out vec4 FragColor;

in VS_OUT
{
   vec3 Pos_WS;
   vec3 Pos_TS;
   vec3 Pos_CS;
   vec3 Normal_WS;
   vec3 Normal_CS;
   vec2 UV;
   vec3 VertexColor;
   vec3 CameraPos_WS;
   vec3 CameraPos_TS; 
   vec3 EyeDirection_CS;
   mat3 TBN;
} IN;

uniform vec4 baseColor = vec4 (1.0, 1.0, 1.0, 1.0);

uniform MaterialData Material;

uniform vec4 ambientLight = vec4(0.05, 0.05, 0.05, 1.0);
uniform int  lightTypes[MAX_LIGHTS];
uniform vec3 lightPosition[MAX_LIGHTS];
uniform vec3 lightColor[MAX_LIGHTS];
uniform vec3 lightDirection[MAX_LIGHTS];

// light attnuation X constant Y linear, Z quadratic
uniform vec3 lightAttenuation[MAX_LIGHTS];
uniform float innerRadius[MAX_LIGHTS];
uniform float outerRadius[MAX_LIGHTS];

uniform int activeLights;
//uniform DirectionalLightData MainLight;
uniform bool BUseLights;

uniform float Gamma = 2.0;// og srgb is 2.2



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
	float specBrightness = pow(max(nDotH, 0.0), specularPower * smoothness);

			
	return specBrightness * (lightColor * 0.3) * attenuation;
}

vec3 GetDirectionalSpecular(float nDotH, vec3 lightColor, float smoothness, float specularPower)
{
	float specBrightness = pow(max(nDotH, 0.0), smoothness * specularPower);

	return specBrightness * (lightColor * 0.3);

}

vec3 GetSpotSpecular(float nDotH, vec3 lightColor, float smoothness, float specularPower, float lightDistance, vec3 attenuation, float cutoff)
{
	return vec3(0,0,0);
}

void main()
{
	FragColor = vec4(0, 0, 0, 1);
	vec3 textureColor = texture(Material.ColorTexture, IN.UV).rgb;
	//textureColor = pow(textureColor, vec3(Gamma));

	//FragColor.xyz += ambientLight.xyz ; // mult in Material ambience when I have it
	if(BUseLights)
	{
	// set up Material stuff that is the same for all light types
		float smoothness = 1;
		vec3 diffuse = vec3(0, 0, 0);
		vec3 specular = vec3(0, 0, 0);

		if(Material.BUseMetallicMap)
		{
			float mapSmoothness = 1.0 - texture(Material.MetallicMap, IN.UV).g;
			//mapSmoothness = Material.bMapIsRoughness ? 1.0 - mapSmoothness : mapSmoothness;
			smoothness = mapSmoothness * Material.Smoothness;
		}
		else
		{
			smoothness = Material.Smoothness;
		}
		smoothness = clamp(smoothness, 0.1, 1.0);

		// ambient lighting
		FragColor.rgb += ambientLight.xyz * textureColor;

		// set up for diffuse and specular lighting

		vec3 N;
		vec3 V;

		if(Material.BUseNormalMap)
		{
			vec3 sampledNormal = texture(Material.NormalMap, IN.UV).rgb;
			sampledNormal = normalize((sampledNormal * 2.0) - 1.0);
			N = sampledNormal;
			V = IN.TBN * normalize(IN.CameraPos_WS.xyz - IN.Pos_WS);
		}
		else
		{
			N = normalize(IN.Normal_WS);
			V = normalize(IN.CameraPos_WS.xyz - IN.Pos_WS);
		}

		vec3 accumulatedDiffuse = vec3(0,0,0);
		vec3 accumulatedSpecular = vec3(0,0,0);

		// light loop
		for(int i = 0; i < activeLights; i++)
		{
			if(lightTypes[i] == 0) // is directional light
			{
				vec3 direction = normalize(-lightDirection[i]);
				if(Material.BUseNormalMap)
				{
					direction = IN.TBN * direction;
				}

				float nDotL = clamp(dot(N, direction), 0.0, 1.0);
				vec3 H = normalize((direction + V));
				float nDotH = clamp(dot(N, H), 0.0, 1.0);

				accumulatedDiffuse += GetDirectionalLuminosity(nDotL, lightColor[i]);
				accumulatedSpecular += GetDirectionalSpecular(nDotH, lightColor[i], smoothness, Material.SpecularPower);
				continue;
			}

			vec3 direction =  normalize(lightPosition[i] - IN.Pos_WS);
			if(Material.BUseNormalMap)
			{
				direction = IN.TBN * direction;
			}

			float nDotL = clamp(dot(N, direction), 0.0, 1.0);

			vec3 H = normalize((direction + V));
			float nDotH = max(dot(N, H), 0.0);
			float lightDistance = length(lightPosition[i] - IN.Pos_WS);

			if(lightTypes[i] == 1) // is point light
			{
				accumulatedDiffuse += GetPointLuminosity(nDotL, lightColor[i], lightDistance, lightAttenuation[i]);
				accumulatedSpecular += GetPointSpecular(nDotH, lightColor[i], smoothness, Material.SpecularPower, lightDistance, lightAttenuation[i]);
				continue;
			}

			if(lightTypes[i] == 2) // is spot light
			{
				float theta = dot(direction, normalize(-lightDirection[i]));
				float epsilon = innerRadius[i] - outerRadius[i];
				float intensity = clamp((theta - outerRadius[i]) / epsilon, 0.0, 1.0);

				accumulatedDiffuse +=  intensity * GetPointLuminosity(nDotL, lightColor[i], lightDistance, lightAttenuation[i]);
				accumulatedSpecular += intensity * GetPointSpecular(nDotH, lightColor[i], smoothness, Material.SpecularPower, lightDistance, lightAttenuation[i]);
				continue;
			}
			
		}	
		FragColor.rgb += (accumulatedDiffuse + accumulatedSpecular) * textureColor;
		//FragColor.xyz = accumulatedSpecular;
		//FragColor.xyz =  accumulatedSpecular;

	}
	else
	{
		FragColor.rgb = textureColor;
	}

	
    FragColor.rgb = clamp(pow(FragColor.rgb, vec3(1.0/Gamma)), 0.0, 1.0);
}