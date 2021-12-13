#pragma once

#include <d3d11.h>
#include "Graphics.h"

namespace ConstantBuffer {
	HRESULT init(Graphics&, ID3D11Buffer**, unsigned),
		setData(Graphics&,  ID3D11Buffer*, void*, unsigned);
}