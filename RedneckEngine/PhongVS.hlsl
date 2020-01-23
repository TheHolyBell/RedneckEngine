#include "Transform.hlsli"

struct VS_OUT
{
	float4 posH : SV_POSITION;
	float4 posW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoord : TEXCOORD;
};

VS_OUT main( float3 pos : POSITION, float3 n : NORMAL, float2 texCoord : TEXCOORD )
{
	VS_OUT vso;

	vso.posH = mul(float4(pos, 1.0f), modelViewProj);
	vso.posW = mul(float4(pos, 1.0f), model);
	vso.normalW = mul(n, (float3x3)model);
	vso.texCoord = texCoord;

	return vso;
}