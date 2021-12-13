#include "Scene.h"
#include "GameMetrics.h"
#include "RasterState.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "ImageTexture.h"
#include "SamplerState.h"

#include <iostream>

// TEMP GLOBALS
float planeVertices[56] = {
	-1.0f, -1.0f, 0.0f, /**/ 1.0f, 0.0f, 0.0f, /**/ 0.0f, 1.0f, 0.0f, /**/ 0.0f, 0.0f, 1.0f, /**/ 0.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, /**/ 1.0f, 0.0f, 0.0f, /**/ 0.0f, 1.0f, 0.0f, /**/ 0.0f, 0.0f, 1.0f, /**/ 0.0f, 1.0f,
	1.0f, -1.0f, 0.0f, /**/ 1.0f, 0.0f, 0.0f, /**/ 0.0f, 1.0f, 0.0f, /**/ 0.0f, 0.0f, 1.0f, /**/ 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f, /**/ 1.0f, 0.0f, 0.0f, /**/ 0.0f, 1.0f, 0.0f, /**/ 0.0f, 0.0f, 1.0f, /**/ 1.0f, 1.0f
};
unsigned numPlaneVertices = 4;
float cubeVertices[24] = {
	-1.0f, 1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f
};
unsigned cubeIndices[14] = { 3, 2, 6, 7, 4, 2, 0, 3, 1, 6, 5, 4, 1, 0 },
	numCubeVertices = 8, numCubeIndices = 14;

Scene::Scene() {
	DirectX::XMFLOAT3 eye{ 1.75f, 0.0f, 1.75f }, focus{}, up{ 0.0f, 0.0f, 1.0f };
	viewMatrix = DirectX::XMMatrixLookAtRH(DirectX::XMLoadFloat3(&eye), DirectX::XMLoadFloat3(&focus), DirectX::XMLoadFloat3(&up));
	projMatrix = DirectX::XMMatrixPerspectiveFovRH(metrics.fov, (float)metrics.width/metrics.height, metrics.screenNear, metrics.screenFar);
}

Scene::~Scene() {
	for(unsigned i = 0; i < 1; i++) if(rasterStates[i]) rasterStates[i]->Release();
	for(unsigned i = 0; i < 2; i++) if(vertexBuffers[i]) vertexBuffers[i]->Release();
	for(unsigned i = 0; i < 1; i++) if(indexBuffers[i]) indexBuffers[i]->Release();
	for(unsigned i = 0; i < 2; i++) if(vertexShaders[i]) vertexShaders[i]->Release();
	for(unsigned i = 0; i < 2; i++) if(inputLayouts[i]) inputLayouts[i]->Release();
	for(unsigned i = 0; i < 2; i++) if(pixelShaders[i]) pixelShaders[i]->Release();
	for(unsigned i = 0; i < 1; i++) if(constantBuffers[i]) constantBuffers[i]->Release();
	for(unsigned i = 0; i < 7; i++) if(textureResources[i]) textureResources[i]->Release();
	for(unsigned i = 0; i < 7; i++) if(textureViews[i]) textureViews[i]->Release();
	for(unsigned i = 0; i < 3; i++) if(samplerStates[i]) samplerStates[i]->Release();
}

HRESULT Scene::init(Graphics& graphics) {
	// initialize raster states
	HRESULT result = RasterState::init(graphics, &rasterStates[0], D3D11_CULL_BACK);
	if(FAILED(result)) return result;

	std::cout << "rasters set up" << std::endl;

	// initialize vertex buffers
	result = VertexBuffer::init(graphics, &vertexBuffers[0], planeVertices, numPlaneVertices, strides[0]);
	if(FAILED(result)) return result;
	result = VertexBuffer::init(graphics, &vertexBuffers[1], cubeVertices, numCubeVertices, strides[1]);
	if(FAILED(result)) return result;

	std::cout << "vertex buffers set up" << std::endl;

	// initialize index buffers
	result = IndexBuffer::init(graphics, &indexBuffers[0], cubeIndices, numCubeIndices, sizeof(unsigned));
	if(FAILED(result)) return result;
	
	std::cout << "index buffers set up" << std::endl;

	// initialize vertex shaders
	result = VertexShader::initForwardPBR(graphics, &vertexShaders[0], &inputLayouts[0]);
	if(FAILED(result)) return result;
	result = VertexShader::initSkyBox(graphics, &vertexShaders[1], &inputLayouts[1]);
	if(FAILED(result)) return result;

	std::cout << "vertex shaders set up" << std::endl;

	// initialize pixel shaders
	result = PixelShader::initForwardPBR(graphics, &pixelShaders[0]);
	if(FAILED(result)) return result;
	result = PixelShader::initSkyBox(graphics, &pixelShaders[1]);
	if(FAILED(result)) return result;

	std::cout << "pixel shaders set up" << std::endl;

	// initialize constant buffers
	result = ConstantBuffer::init(graphics, &constantBuffers[0], sizeof(float)*48);
	if(FAILED(result)) return result;

	std::cout << "constant buffers set up" << std::endl;

	// initialize textures
	// come up with a better way to do this
	std::string material = "tidal-pool2";
	material = "res/materials/" + material + "/";
	result = ImageTexture::initDDS(graphics, &textureResources[0], &textureViews[0], material + "aaax.dds");
	if(FAILED(result)) return result;
	result = ImageTexture::initDDS(graphics, &textureResources[1], &textureViews[1], material + "nnnh.dds");
	if(FAILED(result)) return result;
	result = ImageTexture::initDDS(graphics, &textureResources[2], &textureViews[2], material + "rmax.dds");
	if(FAILED(result)) return result;

	std::string environment = "panorama_map";
	environment = "res/IBL maps/" + environment + "/";
	result = ImageTexture::initDDS(graphics, &textureResources[3], &textureViews[3], environment + "env.dds");
	if(FAILED(result)) return result;
	result = ImageTexture::initDDS(graphics, &textureResources[4], &textureViews[4], environment + "dif.dds");
	if(FAILED(result)) return result;
	result = ImageTexture::initDDS(graphics, &textureResources[5], &textureViews[5], environment + "spec.dds");
	if(FAILED(result)) return result;
	result = ImageTexture::initDDS(graphics, &textureResources[6], &textureViews[6], "res/IBL maps/brdf.dds");
	if(FAILED(result)) return result;

	std::cout << "textures set up" << std::endl;

	// initialize sampler states
	result = SamplerState::init(graphics, &samplerStates[0], D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, -0.1f);
	if(FAILED(result)) return result;
	result = SamplerState::init(graphics, &samplerStates[1], D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f);
	if(FAILED(result)) return result;
	result = SamplerState::init(graphics, &samplerStates[2], D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, 0.0f);
	if(FAILED(result)) return result;

	std::cout << "samplers set up" << std::endl << "initialization complete" << std::endl;

	// pipeline binding

	// rasterization binding

	graphics.getDeviceContext()->RSSetState(rasterStates[0]);

	// input-assembler binding

	// bind vertex buffers
	UINT offsets[2]{};
	graphics.getDeviceContext()->IASetVertexBuffers(0, 2, vertexBuffers, strides, offsets);

	// bind index buffers
	graphics.getDeviceContext()->IASetIndexBuffer(indexBuffers[0], DXGI_FORMAT_R32_UINT, 0);

	// use correct topology
	graphics.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// bind vertex shader constant buffers

	graphics.getDeviceContext()->VSSetConstantBuffers(0, 1, constantBuffers);

	// bind pixel shader textures
	graphics.getDeviceContext()->PSSetShaderResources(0, 7, textureViews);

	// bind pixel shader samplers
	graphics.getDeviceContext()->PSSetSamplers(0, 3, samplerStates);

	return result;
}

void Scene::update(float elapsed) {
	worldMatrix = DirectX::XMMatrixRotationX(rotZ += DirectX::XM_PI/32*elapsed);
	if(rotZ >= DirectX::XM_PIDIV2) rotZ -= DirectX::XM_PI;
}

void Scene::render(Graphics& graphics) {
	// clear the screen
	graphics.prepareTarget();

	// update constant buffers

	// might want to handle failed setData() call
	DirectX::XMMATRIX matrices[3] = { DirectX::XMMatrixTranspose(worldMatrix), DirectX::XMMatrixTranspose(viewMatrix), DirectX::XMMatrixTranspose(projMatrix) };
	ConstantBuffer::setData(graphics, constantBuffers[0], matrices, sizeof(float)*48);
	
	// draw skybox

	// bind input layout
	graphics.getDeviceContext()->IASetInputLayout(inputLayouts[1]);

	// bind vertex shader

	graphics.getDeviceContext()->VSSetShader(vertexShaders[1], 0, 0);

	// bind pixel shader

	graphics.getDeviceContext()->PSSetShader(pixelShaders[1], 0, 0);

	// draw to screen
	graphics.getDeviceContext()->DrawIndexed(numCubeIndices, 0, 0);

	// draw object

	// bind input layout
	graphics.getDeviceContext()->IASetInputLayout(inputLayouts[0]);

	// bind vertex shader

	graphics.getDeviceContext()->VSSetShader(vertexShaders[0], 0, 0);

	// bind pixel shader

	graphics.getDeviceContext()->PSSetShader(pixelShaders[0], 0, 0);

	// draw to screen
	graphics.getDeviceContext()->Draw(numPlaneVertices, 0);
}