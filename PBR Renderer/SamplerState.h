#pragma once

#include <d3d11.h>
#include "Graphics.h"

namespace SamplerState {
	HRESULT init(Graphics&, ID3D11SamplerState**, D3D11_FILTER, D3D11_TEXTURE_ADDRESS_MODE, float);
}