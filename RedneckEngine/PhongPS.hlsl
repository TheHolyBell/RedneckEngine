#include "PointLight.hlsli"
#include "ShaderOps.hlsli"

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

/*float3 Diffuse(
    uniform float3 diffuseColor,
    uniform float diffuseIntensity,
    const in float att,
    const in float3 DirFragToL,
    const in float3 normal)
{
    return diffuseColor * diffuseIntensity * att * max(0.0f, dot(DirFragToL, normal));
}*/


float4 main(VS_OUT input) : SV_TARGET
{
	float3 lightDir = lightPos - input.posW.xyz;

	float4 diffuseColor = diffuseMap.Sample(Sampler, input.texCoord);

	float att = 1.0f / (attConst + attLin * length(lightDir) + attQuad * pow(length(lightDir), 2));

    //float diffuseIntensity = saturate(dot(input.normalW, normalize(lightDir)));



    //return diffuseColor * diffuseIntensity * att;
   // return float4(1.0f, 1.0f, 1.0f, 1.0f);
	return float4(Diffuse(diffuseColor.xyz, diffuseIntensity, att, normalize(lightDir), normalize(input.normalW)) * diffuseColor + ambient, 1.0f);
}