#include "Transform.hlsli"

struct VS_OUT
{
	float4 PosH : SV_POSITION;
	float3 PosL : POSITION;
};

VS_OUT main(float3 pos : POSITION)
{
	VS_OUT vout;

	vout.PosH = mul(float4(pos, 1.0f), modelViewProj).xyww;

	vout.PosL = pos;

	return vout;
}