#include "DepthTexture.h"

HRESULT DepthTexture::init(Graphics& graphics, ID3D11Texture2D** texture, ID3D11DepthStencilView** target, unsigned width, unsigned height) {
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	HRESULT result = graphics.getDevice()->CreateTexture2D(&textureDesc, 0, texture);
	if(FAILED(result)) return result;

	D3D11_DEPTH_STENCIL_VIEW_DESC targetDesc{};
	targetDesc.Format = DXGI_FORMAT_D32_FLOAT;
	targetDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	targetDesc.Flags = 0;
	targetDesc.Texture2D.MipSlice = 0;

	result = graphics.getDevice()->CreateDepthStencilView(*texture, &targetDesc, target);

	return result;
}

HRESULT DepthTexture::init(Graphics& graphics, ID3D11Texture2D** texture, ID3D11DepthStencilView** target, ID3D11ShaderResourceView** resource, unsigned width, unsigned height) {
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	HRESULT result = graphics.getDevice()->CreateTexture2D(&textureDesc, 0, texture);
	if(FAILED(result)) return result;

	D3D11_DEPTH_STENCIL_VIEW_DESC targetDesc{};
	targetDesc.Format = DXGI_FORMAT_D32_FLOAT;
	targetDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	targetDesc.Flags = 0;
	targetDesc.Texture2D.MipSlice = 0;

	result = graphics.getDevice()->CreateDepthStencilView(*texture, &targetDesc, target);
	if(FAILED(result)) return result;

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc{};
	resourceDesc.Format = DXGI_FORMAT_R32_FLOAT;
	resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceDesc.Texture2D.MostDetailedMip = 0;
	resourceDesc.Texture2D.MipLevels = 1;

	result = graphics.getDevice()->CreateShaderResourceView(*texture, &resourceDesc, resource);

	return result;
}