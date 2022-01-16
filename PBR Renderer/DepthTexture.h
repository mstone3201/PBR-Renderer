#pragma once

#include "Graphics.h"

namespace DepthTexture {
	HRESULT init(Graphics&, ID3D11Texture2D**, ID3D11DepthStencilView**, unsigned, unsigned),
		init(Graphics&, ID3D11Texture2D**, ID3D11DepthStencilView**, ID3D11ShaderResourceView**, unsigned, unsigned);
}