Texture2D<float4> preTexture : register(t7);
SamplerState preSampler : register(s2);

struct psInput {
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};

float4 main(psInput input) : SV_TARGET {
	return preTexture.Sample(preSampler, input.tex);
}