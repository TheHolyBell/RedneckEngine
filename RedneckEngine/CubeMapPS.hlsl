TextureCube cubeMap : register(t0);
SamplerState Sampler : register(s0);

struct VS_OUT
{
	float4 PosH : SV_POSITION;
	float3 PosL : POSITION;
};

float4 main(VS_OUT input) : SV_TARGET
{
	return cubeMap.Sample(Sampler, input.PosL);
}