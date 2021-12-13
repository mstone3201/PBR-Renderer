Texture2D<float4> aaaxMap : register(t0);
Texture2D<float4> nnnhMap : register(t1);
Texture2D<float4> rmaxMap : register(t2);
TextureCube<float4> difMap : register(t4);
TextureCube<float4> specMap : register(t5);
Texture2D<float2> brdfLUT : register(t6);
SamplerState mapSampler : register(s0);
SamplerState cubeSampler : register(s1);
SamplerState brdfSampler : register(s2);

struct psInput {
	float4 position : SV_POSITION;
	float3 worldPos : POSITION;
	float3x3 tbn : NORMAL;
	float2 tex : TEXCOORD;
};

static float PI = 3.14159265359f;
static float3 camPos = float3(1.75f, 0.0f, 1.75f);
static float3 lightPos[2] = { float3(-1.75f, 0.0f, 1.75f), float3(5.0f, 1.0f, 3.0f) };
static float3 lightColor[2] = { float3(23.47f, 21.31f, 20.79f), float3(23.47f, 21.31f, 20.79f) };

float3 fresnelSchlick(float cosTheta, float3 F0) {
	return F0 + (1.0f - F0) * pow(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
}

float3 fresnelSchlickRoughness(float cosTheta, float3 F0, float roughness) {
	return F0 + (max(1.0f - roughness, F0) - F0) * pow(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
}

float DistributionGGX(float3 N, float3 H, float roughness) {
	float a = roughness*roughness;
	float a2 = a*a;
	float NdotH = max(dot(N, H), 0.0f);
	float NdotH2 = NdotH*NdotH;

	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
	denom = PI * denom * denom;

	return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
	float r = (roughness + 1.0f);
	float k = (r * r) / 8.0f;

	float num = NdotV;
	float denom = NdotV * (1.0f - k) + k;

	return num / denom;
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness) {
	float NdotV = max(dot(N, V), 0.0f);
	float NdotL = max(dot(N, L), 0.0f);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

float4 main(psInput input) : SV_TARGET {
	float3 albedo = aaaxMap.Sample(mapSampler, input.tex).xyz;
	albedo = pow(albedo, 2.2f); // gamma correct g -> l
	float3 N = nnnhMap.Sample(mapSampler, input.tex).xyz * 2.0f - 1.0f;
	N = normalize(mul(N, input.tbn));
	float3 rma = rmaxMap.Sample(mapSampler, input.tex).xyz;
	float roughness = rma.x;
	float metallic = rma.y;
	float ao = rma.z;

	float w, h, maxLOD;
	specMap.GetDimensions(0, w, h, maxLOD);

	float3 V = normalize(camPos - input.worldPos); // vector pointing from pixel to camera

	float3 F0 = lerp(0.04f, albedo, metallic); // 0 incidence reflection

	// accumulated direct lighting from reflectance equation
	float3 Lo = 0.0f; // accumulated direct lighting
	for(uint i = 0; i < 2; i++) { // 2 point lights
		float3 L = normalize(lightPos[i] - input.worldPos); // vector pointing from pixel to light
		float3 H = normalize(V + L); // vector halfway between camera and light

		// light attenuation
		float distance = length(lightPos[i] - input.worldPos);
		float attenuation = 1.0f / (distance * distance);
		float3 radiance = lightColor[i] * attenuation;

		// Cook-Torrence BRDF
		float3 F = fresnelSchlick(max(dot(H, V), 0.0f), F0);
		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		float3 numerator = F * NDF * G;
		float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.0001f;
		float3 specular = numerator/denominator;

		float3 kD = 1.0f - F;
		kD *= 1.0f - metallic;

		// accumulate radiance
		float NdotL = max(dot(N, L), 0.0f);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	float3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0f), F0, roughness);

	// diffuse irradiance
	float3 kD = 1.0f - F;
	kD *= 1.0f - metallic;
	float3 irradiance = difMap.Sample(cubeSampler, N.xzy).xyz;
	float3 diffuse = irradiance * albedo;

	// specular
	float3 R = reflect(-V, N);
	float3 prefiltered = specMap.SampleLevel(cubeSampler, R.xzy, roughness * maxLOD).xyz;
	float2 brdf = brdfLUT.Sample(brdfSampler, float2(max(dot(N, V), 0.0f), roughness));
	float3 specular = prefiltered * (F * brdf.x + brdf.y);

	// final color
	float3 ambient = (kD * diffuse + specular) * ao;
	float3 color = ambient + Lo;
	color /= color + 1.0f; // reinhard tone map hdr -> ldr
	color = pow(color, 1.0f / 2.2f); // gamma correct l -> g

	return float4(color, 1.0f);
}