#include "PointLight.hlsli"

cbuffer ObjectCBuf
{
    float specularPowerConst;
    bool hasGloss;
    float specularMapWeight;
};

Texture2D diffuseMap;
Texture2D specMap;

SamplerState Sampler;

struct VS_OUT
{
	float4 posH : SV_POSITION;
	float4 posW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoord : TEXCOORD;
};

float4 main(VS_OUT input) : SV_Target
{
	return diffuseMap.Sample(Sampler, input.texCoord);
}