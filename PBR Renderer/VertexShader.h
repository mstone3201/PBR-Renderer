#pragma once

#include <string>
#include "Graphics.h"

namespace VertexShader {
	HRESULT init(Graphics&, ID3D11VertexShader**, ID3D11InputLayout**, D3D11_INPUT_ELEMENT_DESC*, unsigned, std::string),
		init(Graphics&, ID3D11VertexShader**, std::string),
		initForwardPBR(Graphics&, ID3D11VertexShader**, ID3D11InputLayout**),
		initForwardPBR(Graphics&, ID3D11VertexShader**),
		initSkyBox(Graphics&, ID3D11VertexShader**, ID3D11InputLayout**),
		initSkyBox(Graphics&, ID3D11VertexShader**);
}