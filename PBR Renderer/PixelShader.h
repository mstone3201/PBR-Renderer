#pragma once

#include <string>
#include "Graphics.h"

namespace PixelShader {
	HRESULT init(Graphics&, ID3D11PixelShader**, std::string),
		initForwardPBR(Graphics&, ID3D11PixelShader**),
		initSkyBox(Graphics&, ID3D11PixelShader**);
}