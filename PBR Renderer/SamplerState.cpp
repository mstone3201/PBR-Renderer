#include "SamplerState.h"
#include "GameMetrics.h"

HRESULT SamplerState::init(Graphics& graphics, ID3D11SamplerState** samplerState, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE mode, float mipBias) {
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = filter;
	samplerDesc.AddressU = samplerDesc.AddressV = samplerDesc.AddressW = mode;
	samplerDesc.MipLODBias = mipBias;
	samplerDesc.MaxAnisotropy = metrics.anisotropy;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	return graphics.getDevice()->CreateSamplerState(&samplerDesc, samplerState);
}