#include "PointLight.hlsli"
#include "CameraPos.hlsli"

struct VS_OUT
{
    float4 posH : SV_POSITION;
    float4 posW : POSITION;
    float3 normalW : NORMAL;
    float3 tangentW : TANGENT;
    float2 texCoord : TEXCOORD;
};

Texture2D albedoMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D metallicMap : register(t2);
Texture2D roughnessMap : register(t3);
Texture2D aoMap : register(t4);

SamplerState Sampler : register(s0);

static const float PI = 3.14159265359;

float DistributionGGX(float3 normalVec, float3 halfwayVec, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;   // a2 = a^2
    float NdotH = max(dot(normalVec, halfwayVec), 0.0);     // NdotH = normalVec.halfwayVec
    float NdotH2 = NdotH * NdotH;   // NdotH2 = NdotH^2
    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness;
    float k = (r * r) / 2.0f;

    float nom = NdotV;
    float denom = NdotV * (1.0f - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
float3 fresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float3 getNormalFromMap(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
    // Uncompress each component from [0,1] to [-1,1].
    float3 normalT = 2.0f * normalMapSample - 1.0f;

    // Build orthonormal basis.
    float3 N = unitNormalW;
    float3 T = normalize(tangentW - dot(tangentW, N) * N);
    float3 B = cross(N, T);

    float3x3 TBN = float3x3(T, B, N);

    // Transform from tangent space to world space.
    float3 bumpedNormalW = mul(normalT, TBN);

    return bumpedNormalW;
}

float4 main(VS_OUT input) : SV_TARGET
{
    float2 TexCoords = input.texCoord;
    float3 WorldPos = input.posW.xyz;

    float3 albedo = pow(albedoMap.Sample(Sampler, TexCoords).rgb, 2.2);
    float metallic = metallicMap.Sample(Sampler, TexCoords).r;
    float roughness = roughnessMap.Sample(Sampler, TexCoords).r;
    float ao = aoMap.Sample(Sampler, TexCoords).r;

    float3 N = getNormalFromMap(normalMap.Sample(Sampler, TexCoords).xyz, normalize(input.normalW), input.tangentW);
    float3 V = normalize(cameraPos - WorldPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    float3 F0 = float3(0.04, 0.04, 0.04);
    F0 = lerp(F0, albedo, metallic);

    // reflectance equation
    float3 Lo = float3(0.0, 0.0, 0.0);
   
        // calculate per-light radiance
        float3 L = normalize(lightPos - WorldPos);
        float3 H = normalize(V + L);
        float distance = length(lightPos - WorldPos);
        float attenuation = 1.0f / (attConst + attLin * distance + attQuad * pow(distance,2));
        float3 radiance = diffuseColor * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        float3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        float3 nominator = NDF * G * F;
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
        float3 specular = nominator / denominator;

        // kS is equal to Fresnel
        float3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        float3 kD = float3(1.0,1.0,1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again

    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
        float3 ambient = float3(0.03, 0.03, 0.03) * albedo *ao;

    float3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + float3(1.0,1.0,1.0));
    // gamma correct
    color = pow(color, 1.0 / 2.2);

    return float4(color, 1.0f);
}