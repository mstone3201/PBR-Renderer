#pragma once

#include "Graphics.h"

namespace VertexBuffer {
	HRESULT init(Graphics&, ID3D11Buffer**, void*, unsigned, unsigned);
}