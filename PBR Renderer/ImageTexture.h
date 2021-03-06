#pragma once

#include <string>
#include "Graphics.h"

namespace ImageTexture {
	HRESULT init(Graphics&, ID3D11Texture2D**, ID3D11ShaderResourceView**, DXGI_FORMAT, std::string),
		initDDS(Graphics&, ID3D11Texture2D**, ID3D11ShaderResourceView**, std::string),
		initDDS(Graphics&, ID3D11Texture2D**, ID3D11ShaderResourceView**, std::string*, unsigned);
}