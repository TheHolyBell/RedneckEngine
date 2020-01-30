#include "ShaderOps.hlsli"
#include "LightVectorData.hlsli"

#include "PointLight.hlsli"

cbuffer ObjectCBuf
{
	float specularIntensity;
	float specularPower;
	bool normalMapEnabled;
	float padding[1];
};

Texture2D diffuseTexture : register(t0);
Texture2D normalTexture : register(t1);

SamplerState samplerState : register(s0);

struct VS_OUT
{
	float3 posW : Position;
	float3 normal : Normal;
	float3 tan : Tangent;
	float3 bitan : Bitangent;
	float2 texCoord : Texcoord;
	float4 posH : SV_Position;
};

float4 main(VS_OUT input) : SV_TARGET
{
	float3 n = normalize(input.normal);

	if (normalMapEnabled)
		n = MapNormal(normalize(input.tan), normalize(input.bitan), n, input.texCoord, normalTexture, samplerState);

	// fragment to light vector data
	const LightVectorData lv = CalculateLightVectorData(lightPos, input.posW);
	// attenuation
	const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	// diffuse
	const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, n);

	return float4(saturate(diffuse + ambient) * diffuseTexture.Sample(samplerState, input.texCoord).rgb, 1.0f);
	//return diffuseTexture.Sample(samplerState, input.texCoord);
}