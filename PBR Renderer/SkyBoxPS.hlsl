TextureCube<float4> envMap : register(t3);
SamplerState cubeSampler : register(s1);

struct psInput {
	float4 position : SV_POSITION;
	float3 tex : TEXCOORD;
};

float4 main(psInput input) : SV_TARGET {
	float3 envColor = envMap.Sample(cubeSampler, input.tex).xyz;
	//envColor /= envColor+1.0f; // reinhard tone map hrd -> ldr
	envColor = pow(envColor, 1.0f/2.2f); // gamma correct l -> g (should try to bake this in)
	return float4(envColor, 1.0f);
}