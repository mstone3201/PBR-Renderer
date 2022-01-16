#pragma once

#include "Graphics.h"
#include "Camera.h"

//TODO:
// use command lists
// add emissive texture (bbba) (nnnr) (eeem) (h)
// load the geometry vertices in a cleaner way
// displacement mapping and/or pom

class Scene {
	// Raster States
	ID3D11RasterizerState* rasterStates[1]{};

	// Depth States
	ID3D11DepthStencilState* depthStates[2]{};

	// Vertex Buffers
	ID3D11Buffer* vertexBuffers[2]{};
	UINT strides[2]{ sizeof(float) * 15, sizeof(float) * 3 }, numStaticIndices = 0;
	
	// Index Buffers
	ID3D11Buffer* indexBuffers[2]{};

	// Vertex Shaders
	ID3D11VertexShader* vertexShaders[3]{};
	ID3D11InputLayout* inputLayouts[3]{};

	// Pixel Shaders
	ID3D11PixelShader* pixelShaders[3]{};

	// Constant Buffers
	ID3D11Buffer* constantBuffers[3]{};

	// Texture Buffers
	ID3D11Texture2D* imageTextures[7]{};
	ID3D11ShaderResourceView* imageResources[7]{};

	// Sampler States
	ID3D11SamplerState* samplerStates[3]{};

	// Render Targets
	ID3D11Texture2D* renderTextures[1]{};
	ID3D11RenderTargetView* renderTargets[1]{};
	ID3D11ShaderResourceView* renderResources[1]{};
	D3D11_VIEWPORT viewPorts[1]{ { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } };

	// Depth Buffers
	ID3D11Texture2D* depthTextures[1]{};
	ID3D11DepthStencilView* depthTargets[1]{};

	Camera& camera;

	// normalMatrix = transpose(inverse(worldMatrix))
	DirectX::XMMATRIX worldTransform[2] = { DirectX::XMMatrixIdentity(), DirectX::XMMatrixIdentity() };
public:
	Scene(Camera&);
	~Scene();
	HRESULT init(Graphics&);
	void update(float), render(Graphics&);
};