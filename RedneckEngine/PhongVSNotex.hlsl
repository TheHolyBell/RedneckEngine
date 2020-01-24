#include "Transform.hlsli"

struct VS_OUTPUT
{
	float4 posH : SV_Position;
	float3 posW : POSITION;
	float3 normal : NORMAL;
};

VS_OUTPUT main( float3 pos : POSITION, float3 n : NORMAL )
{
	VS_OUTPUT vso;

	vso.posW = mul(float4(pos, 1.0f), model);
	vso.posH = mul(float4(pos, 1.0f), modelViewProj);
	vso.normal = mul(n, (float3x3)model);

	return vso;
}