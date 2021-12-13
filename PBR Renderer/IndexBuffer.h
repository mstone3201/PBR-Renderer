#pragma once

#include "Graphics.h"

namespace IndexBuffer {
	HRESULT init(Graphics&, ID3D11Buffer**, void*, unsigned, unsigned);
}