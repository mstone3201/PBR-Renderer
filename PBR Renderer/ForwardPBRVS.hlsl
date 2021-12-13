cbuffer matrices : register(b0) {
	float4x4 worldMatrix, viewMatrix, projMatrix;
};

struct vsInput {
	float3 position : POSITION;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD;
};

struct vsOutput {
	float4 position : SV_POSITION;
	float3 worldPos : POSITION;
	float3x3 tbn : NORMAL;
	float2 tex : TEXCOORD;
};

vsOutput main(vsInput input) {
	vsOutput output;
	output.position = mul(float4(input.position, 1.0f), worldMatrix);
	output.worldPos = output.position.xyz;
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projMatrix);
	float3 T = normalize(mul(input.tangent, (float3x3)worldMatrix));
	float3 B = normalize(mul(input.binormal, (float3x3)worldMatrix));
	float3 N = normalize(mul(input.normal, (float3x3)worldMatrix));
	output.tbn = float3x3(T, B, N);
	output.tex = input.tex;
	return output;
}