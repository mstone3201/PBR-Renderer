#pragma once

#include <string>
#include "Graphics.h"

namespace Geometry {
	HRESULT initMesh(Graphics&, ID3D11Buffer**, ID3D11Buffer**, unsigned&, std::string*&, unsigned&, std::string);
}