struct vsOutput {
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};

vsOutput main(float2 position : POSITION) {
	vsOutput output;
	output.position = float4(position, 0.0f, 1.0f);
	output.tex = position * 0.5f + 0.5f;
	output.tex.y = 1.0f - output.tex.y;
	return output;
}