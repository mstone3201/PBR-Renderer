#pragma once

#include "Graphics.h"

namespace DepthState {
	HRESULT init(Graphics&, ID3D11DepthStencilState**, bool);
}