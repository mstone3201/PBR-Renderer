/* PBR */
// BRDF used by Unreal Engine 4: https://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
// Cook-Torrance reflectance model: https://graphics.pixar.com/library/ReflectanceModel/paper.pdf
// Substance Designer PBR reference code (with substance installed): C:\Program Files\Allegorithmic\Adobe Substance 3D Designer\resources\view3d\shaders\physically_metallic_roughness\fs.glsl
// fresnel equation with roughness term: https://seblagarde.wordpress.com/2011/08/17/hello-world/
// helpful resource: https://seblagarde.files.wordpress.com/2015/07/course_notes_moving_frostbite_to_pbr_v32.pdf
// helpful resource: https://learnopengl.com/PBR/Theory

#include "color.hlsl"

#define PI 3.1415926535897932384626433832795
#define INV_PI 0.31830988618379067153776752674503

cbuffer viewTransform : register(b0) {
	float4x4 viewMatrix, invViewMatrix;
};

Texture2DArray<float4> bbbxMap : register(t0);
Texture2DArray<float4> nnnhMap : register(t1);
Texture2DArray<float4> rmaxMap : register(t2);
TextureCube<float4> difIBL : register(t4);
TextureCube<float4> specIBL : register(t5);
Texture2D<float2> brdfLUT : register(t6);

SamplerState mapSampler : register(s0);
SamplerState cubeSampler : register(s1);
SamplerState brdfSampler : register(s2);

struct psInput {
	float4 position : SV_POSITION;
	float3 worldPos : POSITION;
	float3x3 tbn : NORMAL;
	float3 tex : TEXCOORD;
};

static float3 lightPos[2] = { float3(-1.75f, 0.0f, 1.75f), float3(5.0f, 1.0f, 3.0f) };
static float3 lightColor[2] = { float3(13.47f, 11.31f, 10.79f), float3(13.47f, 11.31f, 10.79f) };
static float lightRadius[2] = { 10.0f, 10.0f };

// Specular D
float D(float NH, float roughness) {
	float a = roughness * roughness;
	float a2 = a * a;
	float div = NH * NH * (a2 - 1.0f) + 1.0f;
	return a2 / max(PI * div * div, 1e-8f);
}

// Specular G - optimize out NL * NV from the numerator
float G1(float NX, float k) {
	return 1.0f / (NX * (1.0f - k) + k);
}

float G(float NL, float NV, float roughness) {
	float rplus = roughness + 1.0f;
	float k = rplus * rplus / 8.0f;
	return G1(NL, k) * G1(NV, k);
}

// Specular F
float3 F(float VH, float3 F0) {
	return F0 + ldexp(1.0f - F0, (-5.55473f * VH - 6.98316f) * VH);
}

// Fresnel with fudged roughness term
float3 F_roughness(float3 NV, float3 F0, float3 roughness) {
	return F0 + ldexp(saturate(1.0f - roughness - F0), (-5.55473f * NV - 6.98316f) * NV);
}

float4 main(psInput input) : SV_TARGET {
	float3 baseColor = bbbxMap.Sample(mapSampler, input.tex).xyz;
	baseColor = srgb_to_linear(baseColor);
	float3 N = nnnhMap.Sample(mapSampler, input.tex).xyz * 2.0f - 1.0f;
	// normal vector
	N = normalize(mul(N, input.tbn));
	float3 rma = rmaxMap.Sample(mapSampler, input.tex).xyz;
	float roughness = rma.x;
	float metallic = rma.y;
	float ao = rma.z;

	float3 diffColor = baseColor * (1.0f - metallic);
	float3 specColor = lerp(0.04f, baseColor, metallic); // Substance uses (0.08 * specularLevel) where 0.5 is the default for dielectric materials

	// vector pointing to the camera
	float3 V = normalize(invViewMatrix[3].xyz - input.worldPos);

	float NV = saturate(dot(N, V));

	// accumulate analytical light contribution
	float3 Ir = 0.0f;
	for(uint i = 0; i < 2; i++) {
		// light vector point from pixel to light
		float3 PtoL = lightPos[i] - input.worldPos;
		float distance = length(PtoL);
		float3 L = PtoL / distance;
		// halfway vector between V and L
		float3 H = normalize(V + L);

		float NH = saturate(dot(N, H));
		float NL = saturate(dot(N, L));
		float VH = saturate(dot(V, H));

		// specular contribution
		float3 Fx = F(VH, specColor); // F0 = specColor
		float3 Rs = D(NH, roughness) * G(NL, NV, roughness) * Fx / 4.0f; // optimize out NL * NV from the denominator

		// diffuse contribution
		float3 Rd = diffColor * INV_PI;
		float3 d = 1.0f - Fx;
		
		// total reflectance
		float3 R = Rs + d * Rd; // s = Fx, which is already in the numerator of Rs so it is omitted

		// light intensity - point light
		float window = distance / lightRadius[i];
		float window2 = window * window;
		float numsqrt = saturate(1.0f - window2 * window2);
		float falloff = numsqrt * numsqrt / (distance * distance + 1.0f);
		float3 I = lightColor[i] * falloff;

		// total contribution from this light
		Ir += I * NL * R;
	}

	// image-based lighting contribution
	float specW, specH, specLODs;
	specIBL.GetDimensions(0, specW, specH, specLODs);

	float3 Fx = F_roughness(NV, specColor, roughness); // F0 = specColor

	// specular contribution
	float3 R = 2.0f * NV * N - V; // reflect(-V, N)
	float3 Is = specIBL.SampleLevel(cubeSampler, R.xzy, roughness * specLODs).xyz;
	float2 brdf = brdfLUT.Sample(brdfSampler, float2(NV, roughness));
	float3 Rs = Is * (Fx * brdf.x + brdf.y);

	// diffuse contribution
	float3 Id = difIBL.Sample(cubeSampler, N.xzy).xyz;
	float3 Rd = diffColor * Id;
	float3 d = 1.0f - Fx;
	
	// total contribution from image-based lighting
	Ir += (Rs + d * Rd) * ao;

	// final color
	float3 color = aces_tonemap(Ir);
	color = linear_to_srgb(color); // switch to using an srgb buffer when render targets are set up (can't make the back buffer srgb since the flip model is being used)
	
	return float4(color, 1.0f);
}