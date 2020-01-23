#include "PointLight.hlsli"

cbuffer ObjectCBuf
{
	float specularIntensity;
	float specularPower;
	bool normalMapEnabled;
	float padding;
};

Texture2D diffuseTexture : register(t0);
Texture2D normalTexture : register(t1);

SamplerState samplerState : register(s0);

struct VS_OUT
{
	float4 posH : SV_POSITION;
	float4 posW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoord : TEXCOORD;
};

float4 main(VS_OUT input) : SV_TARGET
{
	input.normalW = normalize(input.normalW);
	

	return diffuseTexture.Sample(samplerState, input.texCoord);
}