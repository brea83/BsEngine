#version 450 core

// based on the "perfect grid shader" article 
// https://bgolus.medium.com/the-best-darn-grid-shader-yet-727f9278b9d8
// and on the axis colered variation here https://gist.github.com/bgolus/3a561077c86b5bfead0d6cc521097bae
// the axis colored variation is written for unity and thus hlsl I believe

//struct MaterialData
//{
//	sampler2D ColorTexture;
//	vec3 BaseColor;
//
//	sampler2D NormalMap;
//	bool BUseNormalMap;
//
//	sampler2D MetallicMap;
//	bool BUseMetallicMap;
//	bool BMapIsRoughness;
//
//	vec3 Ao;
//	
//	float Smoothness;
//	float SpecularPower;
//};

in VS_OUT
{
   //vec3 Pos_WS;
   //vec3 Pos_CS;
   vec4 UV;
   int MajorGridDiv;

} IN;

out vec4 FragColor;

//uniform MaterialData Material;
uniform int PlaneAxisEnum; // 0, 1, 2 == XZ, YZ, YX

// all line widths should range between 0 and 1
uniform float AxisLineWidth = 0.05;
uniform float MajorLineWidth = 0.03;
uniform float MinorLineWidth = 0.01;

uniform vec4 MajorLineColor = vec4(1, 1, 1, 1);
uniform vec4 MinorLineColor = vec4(0.5, 0.5, 0.5, 1);

struct AxisColors 
{
	vec4 Main;
	vec4 Dash;
};

uniform AxisColors XAxisColor = AxisColors(vec4(1, 0, 0, 1), vec4(0.5, 0, 0, 1));
uniform AxisColors YAxisColor = AxisColors(vec4(0, 1, 0, 1), vec4(0, 0.5, 0, 1));
uniform AxisColors ZAxisColor = AxisColors(vec4(0, 0, 1, 1), vec4(0, 0, 0.5, 1));
uniform vec4 CenterColor = vec4(1, 1, 1, 1);

uniform float AxisDashScale = 1.33;

uniform vec4 BaseColor = vec4(1, 1, 1, 0);

float SaturateF(float value)
{
	return clamp(value, 0.0, 1.0);
};

vec2 SaturateV2(vec2 value)
{
	return vec2(clamp(value.x, 0.0, 1.0), clamp(value.y, 0.0, 1.0));
}

vec3 SaturateV3(vec3 value)
{
	return vec3(clamp(value.x, 0.0, 1.0), clamp(value.y, 0.0, 1.0), clamp(value.z, 0.0, 1.0));
}

vec4 SaturateV4(vec4 value)
{
	return vec4(clamp(value.x, 0.0, 1.0), clamp(value.y, 0.0, 1.0), clamp(value.z, 0.0, 1.0), clamp(value.w, 0.0, 1.0));
}



void main()
{
	vec4 uvDDXY = vec4(dFdx(IN.UV.xy), dFdy(IN.UV.xy));
	vec2 uvDeriv = vec2(length(uvDDXY.xz), length(uvDDXY.yw));

	// Line WIDTHS
	// ----------------------------------
	// axis lines
	float asixlineWidth = max(MajorLineWidth, AxisLineWidth);
	vec2 axisDrawWidth = max(vec2(AxisLineWidth), uvDeriv);

	vec2 axisLineAA = uvDeriv * 1.5;
	vec2 axisLines2 = smoothstep(axisDrawWidth + axisLineAA, axisDrawWidth - axisLineAA, abs(IN.UV.zw * 2.0));
	axisLines2 *= SaturateV2( vec2(AxisLineWidth) / axisDrawWidth);

	// major lines
	float div = max(2.0, round(IN.MajorGridDiv));
	vec2 majorUVDeriv = uvDeriv / div;
	float majorLineWidth = MajorLineWidth / div;
	vec2 majorDrawWidth;
	majorDrawWidth.x = clamp(majorLineWidth, majorUVDeriv.x, 0.5);
	majorDrawWidth.y = clamp(majorLineWidth, majorUVDeriv.y, 0.5);

	vec2 majorLineAA = majorUVDeriv * 1.5;
	vec2 majorGridUV = 1.0 - abs(fract(IN.UV.xy / div) * 2.0 -1.0);
	vec2 majorAxisOffset = (1.0 - SaturateV2(abs(IN.UV.xw / div * 2.0))) * 2.0;
	majorGridUV += majorAxisOffset; // adjusts uvs so center axis line is skipped

	vec2 majorGrid2 = smoothstep(majorDrawWidth + majorLineAA, majorDrawWidth - majorLineAA, majorGridUV);
	majorGrid2 *= SaturateV2(majorLineWidth / majorDrawWidth);
	majorGrid2 = SaturateV2(majorGrid2 - axisLines2);
	majorGrid2 = mix(majorGrid2, vec2(majorLineWidth), SaturateV2(majorUVDeriv * 2.0 - 1.0));

	// minor lines

	float minorLineWidth = min(MinorLineWidth, majorLineWidth);
	bool minorInvertLine = minorLineWidth > 0.5;
	float minorTargetWidth = minorInvertLine ? 1.0 - minorLineWidth : minorLineWidth;
	
	vec2 minorDrawWidth;
	minorDrawWidth.x = clamp(minorTargetWidth, uvDeriv.x, 0.5);
	minorDrawWidth.y = clamp(minorTargetWidth, uvDeriv.y, 0.5);

	vec2 minorLineAA = uvDeriv * 1.5;
	vec2 minorGridUV = abs(fract(IN.UV.xy) * 2.0 - 1.0);
	minorGridUV  = minorInvertLine ? minorGridUV : 1.0 - minorGridUV;

	vec2 minorMajorOffset = vec2(1.0 - SaturateV2((1.0 - abs(fract(IN.UV.zw / div) * 2.0 - 1.0)) * div)) * 2.0;
	
	minorGridUV += minorMajorOffset; // adjust uvs so major division lines are skipped

	vec2 minorGrid2 = smoothstep(minorDrawWidth + minorLineAA, minorDrawWidth - minorLineAA, minorGridUV);
	minorGrid2 *= SaturateV2(vec2(minorTargetWidth) / minorDrawWidth);
	minorGrid2 = SaturateV2(minorGrid2 - axisLines2); // hack to remove the axis lines from these calcs?
	minorGrid2 = mix(minorGrid2, vec2(minorTargetWidth), SaturateV2(uvDeriv * 2.0 - 1.0));
	minorGrid2 = minorInvertLine ? 1.0 - minorGrid2 : minorGrid2;
	minorGrid2 = abs(IN.UV.z) > 0.5 || abs(IN.UV.w) > 0.5 ? minorGrid2 : vec2(0.0); // may need to split this out to behave differently on each axis, not sure


	float minorGrid = mix(minorGrid2.x, 1.0, minorGrid2.y);
	float majorGrid = mix(majorGrid2.x, 1.0, majorGrid2.y);

	vec2 axisDashUV = abs(fract((IN.UV.zw + vec2(asixlineWidth) * 0.5) * AxisDashScale) * 2.0 -1.0) - 0.5;
	vec2 axisDashDeriv = uvDeriv * AxisDashScale * 1.5;
	vec2 axisDash = smoothstep(-axisDashDeriv, axisDashDeriv, axisDashUV);
	axisDash = abs(IN.UV.z) > 0.5 || abs(IN.UV.w) > 0.5 ? axisDash : 1.0;

	// Line COLORS
	// ----------------------------------

	vec4 aAxisColor;
	vec4 aAxisDashColor;

	vec4 bAxisColor;
	vec4 bAxisDashColor;
	
	if( PlaneAxisEnum == 0) // XZ
	{
		aAxisColor = XAxisColor.Main;
		aAxisDashColor = XAxisColor.Dash;

		bAxisColor = ZAxisColor.Main;
		bAxisDashColor = ZAxisColor.Dash;
	}
	else if( PlaneAxisEnum == 1) // YZ
	{
		aAxisColor = YAxisColor.Main;
		aAxisDashColor = YAxisColor.Dash;

		bAxisColor = ZAxisColor.Main;
		bAxisDashColor = ZAxisColor.Dash;
	}
	else if( PlaneAxisEnum == 2) // XY
	{
		aAxisColor = XAxisColor.Main;
		aAxisDashColor = XAxisColor.Dash;

		bAxisColor = YAxisColor.Main;
		bAxisDashColor = YAxisColor.Dash;
	}

	aAxisColor = mix(aAxisDashColor, aAxisColor, axisDash.y);
	bAxisColor = mix(bAxisDashColor, bAxisColor, axisDash.x);
	aAxisColor = mix(aAxisColor, CenterColor, axisLines2.y);

	vec4 axisLines = mix(bAxisColor * axisLines2.y, aAxisColor, axisLines2.x);

	FragColor = mix(BaseColor, MinorLineColor, minorGrid * MinorLineColor.a);
	FragColor = mix(FragColor, MajorLineColor, majorGrid * MajorLineColor.a);
	FragColor = FragColor * (1.0 - axisLines.a) + axisLines;

	if(FragColor.a < 0.1)
        discard;
}