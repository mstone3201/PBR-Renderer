#pragma once

#include "Graphics.h"

namespace RasterState {
	HRESULT init(Graphics&, ID3D11RasterizerState**, D3D11_CULL_MODE);
}