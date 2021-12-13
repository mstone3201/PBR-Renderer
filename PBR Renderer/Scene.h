#pragma once

#include <DirectXMath.h>
#include "Graphics.h"

class Scene {
	// Raster States
	ID3D11RasterizerState* rasterStates[1]{};

	// Vertex Buffers
	ID3D11Buffer* vertexBuffers[2]{};
	UINT strides[2]{ sizeof(float)*14, sizeof(float)*3 };
	
	// Index Buffers
	ID3D11Buffer* indexBuffers[1]{};

	// Vertex Shaders
	ID3D11VertexShader* vertexShaders[2]{};
	ID3D11InputLayout* inputLayouts[2]{};

	// Pixel Shaders
	ID3D11PixelShader* pixelShaders[2]{};

	// Constant Buffers
	ID3D11Buffer* constantBuffers[1]{};

	// Texture Buffers
	ID3D11Texture2D* textureResources[7]{};
	ID3D11ShaderResourceView* textureViews[7]{};

	// Sampler States
	ID3D11SamplerState* samplerStates[3]{};

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity(), viewMatrix, projMatrix;
	float rotZ = 0.0f;
public:
	Scene();
	~Scene();
	HRESULT init(Graphics&);
	void update(float), render(Graphics&);
};