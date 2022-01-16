#pragma once

#include <string>
#include <vector>
#include "Graphics.h"

namespace Material {
	HRESULT init(Graphics&, ID3D11Texture2D**, ID3D11ShaderResourceView**, std::string),
		init(Graphics&, ID3D11Texture2D**, ID3D11ShaderResourceView**, std::string*, unsigned);
}