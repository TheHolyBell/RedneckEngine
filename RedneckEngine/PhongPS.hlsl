#include "PointLight.hlsli"

cbuffer ObjectCBuf
{
    float specularIntensity;
    float specularPower;
    float padding[2];
};

Texture2D diffuseMap : register(t0);
SamplerState Sampler : register(s0);

struct VS_OUT
{
	float4 posH : SV_POSITION;
	float4 posW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoord : TEXCOORD;
};

float4 main(VS_OUT input) : SV_TARGET
{
	return diffuseMap.Sample(Sampler, input.texCoord);
}