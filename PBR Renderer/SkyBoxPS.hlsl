#include "color.hlsl"

TextureCube<float4> envMap : register(t3);
SamplerState cubeSampler : register(s1);

struct psInput {
	float4 position : SV_POSITION;
	float3 tex : TEXCOORD;
};

float4 main(psInput input) : SV_TARGET {
	float3 envColor = envMap.Sample(cubeSampler, input.tex).xyz;
	envColor = aces_tonemap(envColor);
	envColor = linear_to_srgb(envColor);
	return float4(envColor, 1.0f);
}