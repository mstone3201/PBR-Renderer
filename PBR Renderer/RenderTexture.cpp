#include "RenderTexture.h"

HRESULT RenderTexture::init(Graphics& graphics, ID3D11Texture2D** texture, ID3D11RenderTargetView** target, ID3D11ShaderResourceView** resource, unsigned width, unsigned height, DXGI_FORMAT format) {
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = format;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	HRESULT result = graphics.getDevice()->CreateTexture2D(&textureDesc, 0, texture);
	if(FAILED(result)) return result;

	D3D11_RENDER_TARGET_VIEW_DESC targetDesc{};
	targetDesc.Format = textureDesc.Format;
	targetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	targetDesc.Texture2D.MipSlice = 0;

	result = graphics.getDevice()->CreateRenderTargetView(*texture, &targetDesc, target);
	if(FAILED(result)) return result;

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc{};
	resourceDesc.Format = textureDesc.Format;
	resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceDesc.Texture2D.MostDetailedMip = 0;
	resourceDesc.Texture2D.MipLevels = 1;

	result = graphics.getDevice()->CreateShaderResourceView(*texture, &resourceDesc, resource);

	return result;
}