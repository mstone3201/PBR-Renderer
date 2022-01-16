cbuffer viewTransform : register(b0) {
	float4x4 viewMatrix, invViewMatrix;
};
cbuffer projTransform : register(b1) {
	float4x4 projMatrix, invProjMatrix;
}

struct vsOutput {
	float4 position : SV_POSITION;
	float3 tex : TEXCOORD;
};

vsOutput main(float3 position : POSITION) {
	vsOutput output;
	output.position = mul(float4(position, 0.0f), viewMatrix); // ignore view matrix translation
	output.position.w = 1.0f; // allow for projection matrix translation
	output.position = mul(output.position, projMatrix);
	output.tex = position.xzy; // make the y direction point up to get the right sample coords
	return output;
}