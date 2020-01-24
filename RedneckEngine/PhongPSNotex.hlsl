#include "PointLight.hlsli"

struct VS_OUTPUT
{
	float4 posH : SV_Position;
	float3 posW : POSITION;
	float3 normal : NORMAL;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
	return float4(1.0f, 0.0f, 1.0f, 1.0f);
}