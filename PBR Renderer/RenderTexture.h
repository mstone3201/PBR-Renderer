#pragma once

#include "Graphics.h"

namespace RenderTexture {
	HRESULT init(Graphics&, ID3D11Texture2D**, ID3D11RenderTargetView**, ID3D11ShaderResourceView**, unsigned, unsigned, DXGI_FORMAT);
}