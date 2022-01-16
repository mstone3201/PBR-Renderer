#include "Scene.h"
#include "GameMetrics.h"
#include "RasterState.h"
#include "DepthState.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "ImageTexture.h"
#include "SamplerState.h"
#include "Geometry.h"
#include "Material.h"
#include "RenderTexture.h"
#include "DepthTexture.h"

#include <iostream>

float geometryVertices[24] = {
	-1.0f, 1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f
};
unsigned geometryIndices[18] = { 3, 2, 6, 7, 4, 2, 0, 3, 1, 6, 5, 4, 1, 0, 0, 2, 1, 3 },
	numGeometryVertices = 8, cubeStartIndex = 0, numCubeIndices = 14, planeStartIndex = 14, numPlaneIndices = 4, numGeometryIndices = 18;

Scene::Scene(Camera& camera) : camera(camera) {
	viewPorts[0].Width = (float)metrics.width;
	viewPorts[0].Height = (float)metrics.height;
}

Scene::~Scene() {
	for(unsigned i = 0; i < 1; i++) if(rasterStates[i]) rasterStates[i]->Release();
	for(unsigned i = 0; i < 2; i++) if(depthStates[i]) depthStates[i]->Release();
	for(unsigned i = 0; i < 2; i++) if(vertexBuffers[i]) vertexBuffers[i]->Release();
	for(unsigned i = 0; i < 2; i++) if(indexBuffers[i]) indexBuffers[i]->Release();
	for(unsigned i = 0; i < 3; i++) if(vertexShaders[i]) vertexShaders[i]->Release();
	for(unsigned i = 0; i < 3; i++) if(inputLayouts[i]) inputLayouts[i]->Release();
	for(unsigned i = 0; i < 3; i++) if(pixelShaders[i]) pixelShaders[i]->Release();
	for(unsigned i = 0; i < 3; i++) if(constantBuffers[i]) constantBuffers[i]->Release();
	for(unsigned i = 0; i < 7; i++) if(imageTextures[i]) imageTextures[i]->Release();
	for(unsigned i = 0; i < 7; i++) if(imageResources[i]) imageResources[i]->Release();
	for(unsigned i = 0; i < 3; i++) if(samplerStates[i]) samplerStates[i]->Release();
	for(unsigned i = 0; i < 1; i++) if(renderTextures[i]) renderTextures[i]->Release();
	for(unsigned i = 0; i < 1; i++) if(renderTargets[i]) renderTargets[i]->Release();
	for(unsigned i = 0; i < 1; i++) if(renderResources[i]) renderResources[i]->Release();
	for(unsigned i = 0; i < 1; i++) if(depthTextures[i]) depthTextures[i]->Release();
	for(unsigned i = 0; i < 1; i++) if(depthTargets[i]) depthTargets[i]->Release();
}

HRESULT Scene::init(Graphics& graphics) {
	// initialize raster states
	HRESULT result = RasterState::init(graphics, &rasterStates[0], D3D11_CULL_BACK);
	if(FAILED(result)) return result;

	std::cout << "raster states set up" << std::endl;

	// initialize depth states
	result = DepthState::init(graphics, &depthStates[0], false);
	if(FAILED(result)) return result;
	result = DepthState::init(graphics, &depthStates[1], true);
	if(FAILED(result)) return result;

	std::cout << "depth states set up" << std::endl;

	// initialize vertex and index buffers
	std::string* materials;
	unsigned numMaterials;
	result = Geometry::initMesh(graphics, &vertexBuffers[0], &indexBuffers[0], numStaticIndices, materials, numMaterials, "../../res/meshes/test_level.mesh");
	if(FAILED(result)) return result;

	result = VertexBuffer::init(graphics, &vertexBuffers[1], geometryVertices, numGeometryVertices * strides[1]);
	if(FAILED(result)) return result;
	result = IndexBuffer::init(graphics, &indexBuffers[1], geometryIndices, numGeometryIndices * sizeof(unsigned));
	if(FAILED(result)) return result;
	
	std::cout << "vertex and index buffers set up" << std::endl;

	// initialize vertex shaders
	result = VertexShader::initForwardPBR(graphics, &vertexShaders[0], &inputLayouts[0]);
	if(FAILED(result)) return result;
	result = VertexShader::initSkyBox(graphics, &vertexShaders[1], &inputLayouts[1]);
	if(FAILED(result)) return result;
	result = VertexShader::initPost(graphics, &vertexShaders[2], &inputLayouts[2]);
	if(FAILED(result)) return result;

	std::cout << "vertex shaders set up" << std::endl;

	// initialize pixel shaders
	result = PixelShader::initForwardPBR(graphics, &pixelShaders[0]);
	if(FAILED(result)) return result;
	result = PixelShader::initSkyBox(graphics, &pixelShaders[1]);
	if(FAILED(result)) return result;
	result = PixelShader::initPost(graphics, &pixelShaders[2]);
	if(FAILED(result)) return result;

	std::cout << "pixel shaders set up" << std::endl;

	// initialize constant buffers
	result = ConstantBuffer::init(graphics, &constantBuffers[0], sizeof(float) * 32);
	if(FAILED(result)) return result;
	result = ConstantBuffer::init(graphics, &constantBuffers[1], sizeof(float) * 32);
	if(FAILED(result)) return result;
	result = ConstantBuffer::init(graphics, &constantBuffers[2], sizeof(float) * 32);
	if(FAILED(result)) return result;

	std::cout << "constant buffers set up" << std::endl;

	// initialize textures
	result = Material::init(graphics, imageTextures, imageResources, materials, numMaterials);
	delete[] materials;
	if(FAILED(result)) return result;

	std::string environment = "panorama_map";
	environment = "../../res/IBL maps/" + environment + "/";
	result = ImageTexture::initDDS(graphics, &imageTextures[3], &imageResources[3], environment + "env.dds");
	if(FAILED(result)) return result;
	result = ImageTexture::initDDS(graphics, &imageTextures[4], &imageResources[4], environment + "dif.dds");
	if(FAILED(result)) return result;
	result = ImageTexture::initDDS(graphics, &imageTextures[5], &imageResources[5], environment + "spec.dds");
	if(FAILED(result)) return result;

	result = ImageTexture::initDDS(graphics, &imageTextures[6], &imageResources[6], "../../res/IBL maps/brdf.dds");
	if(FAILED(result)) return result;

	std::cout << "textures set up" << std::endl;

	// initialize render targets
	result = RenderTexture::init(graphics, &renderTextures[0], &renderTargets[0], &renderResources[0], metrics.width, metrics.height, DXGI_FORMAT_R8G8B8A8_UNORM);
	if(FAILED(result)) return result;

	std::cout << "render targets set up" << std::endl;

	// initialize depth buffers
	result = DepthTexture::init(graphics, &depthTextures[0], &depthTargets[0], metrics.width, metrics.height);
	if(FAILED(result)) return result;

	std::cout << "depth buffers set up" << std::endl;

	// initialize sampler states
	result = SamplerState::init(graphics, &samplerStates[0], D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, -0.1f);
	if(FAILED(result)) return result;
	result = SamplerState::init(graphics, &samplerStates[1], D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f);
	if(FAILED(result)) return result;
	result = SamplerState::init(graphics, &samplerStates[2], D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, 0.0f);
	if(FAILED(result)) return result;

	std::cout << "samplers set up" << std::endl << "initialization complete" << std::endl;

	// pipeline & first pass binding

	// rasterization binding

	graphics.getDeviceContext()->RSSetState(rasterStates[0]);

	// input-assembler binding

	// bind vertex buffers
	UINT offsets[2]{};
	graphics.getDeviceContext()->IASetVertexBuffers(0, 2, vertexBuffers, strides, offsets);

	// bind starting index buffer
	graphics.getDeviceContext()->IASetIndexBuffer(indexBuffers[1], DXGI_FORMAT_R32_UINT, 0);

	// use starting topology
	graphics.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// output-merger binding

	// start with depth testing disabled
	graphics.getDeviceContext()->OMSetDepthStencilState(depthStates[0], 0);

	// vertex shader binding

	// bind vertex shader constant buffers
	graphics.getDeviceContext()->VSSetConstantBuffers(0, 3, constantBuffers);

	// pixel shader binding

	// bind pixel shader constant buffers
	graphics.getDeviceContext()->PSSetConstantBuffers(0, 1, constantBuffers);

	// bind pixel shader textures
	graphics.getDeviceContext()->PSSetShaderResources(0, 7, imageResources);

	// bind pixel shader samplers
	graphics.getDeviceContext()->PSSetSamplers(0, 3, samplerStates);

	// update permanent constant buffers
	result = ConstantBuffer::setData(graphics, constantBuffers[1], camera.projTransform, sizeof(float) * 32);
	if(FAILED(result)) return result;
	ConstantBuffer::setData(graphics, constantBuffers[2], worldTransform, sizeof(float) * 32);

	return result;
}

void Scene::update(float elapsed) {}

void Scene::render(Graphics& graphics) {
	// update constant buffers
	HRESULT result = ConstantBuffer::setData(graphics, constantBuffers[0], camera.viewTransform, sizeof(float) * 32);
	if(FAILED(result)) return;

	// rasterization binding

	graphics.getDeviceContext()->RSSetViewports(1, viewPorts);

	// output-merger binding

	graphics.getDeviceContext()->OMSetRenderTargets(1, renderTargets, depthTargets[0]);

	// draw skybox

	// indexBuffers[1] bound here

	// trianglestrip topology used here

	// bind input layout
	graphics.getDeviceContext()->IASetInputLayout(inputLayouts[1]);

	// bind vertex shader
	graphics.getDeviceContext()->VSSetShader(vertexShaders[1], 0, 0);

	// bind pixel shader
	graphics.getDeviceContext()->PSSetShader(pixelShaders[1], 0, 0);

	// depth testing disabled here

	// draw to screen
	graphics.getDeviceContext()->DrawIndexed(numCubeIndices, 0, 0);

	// draw scene

	// clear depth buffer
	graphics.getDeviceContext()->ClearDepthStencilView(depthTargets[0], D3D11_CLEAR_DEPTH, 1.0f, 0);

	// bind index buffer
	graphics.getDeviceContext()->IASetIndexBuffer(indexBuffers[0], DXGI_FORMAT_R32_UINT, 0);

	// use correct topology
	graphics.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// bind input layout
	graphics.getDeviceContext()->IASetInputLayout(inputLayouts[0]);

	// bind vertex shader
	graphics.getDeviceContext()->VSSetShader(vertexShaders[0], 0, 0);

	// bind pixel shader
	graphics.getDeviceContext()->PSSetShader(pixelShaders[0], 0, 0);

	// enable depth testing
	graphics.getDeviceContext()->OMSetDepthStencilState(depthStates[1], 0);

	// draw to screen
	graphics.getDeviceContext()->DrawIndexed(numStaticIndices, 0, 0);

	// post-processing

	// rasterization binding

	graphics.getDeviceContext()->RSSetViewports(1, &graphics.viewPort);

	// output-merger binding

	ID3D11RenderTargetView* views[1] = { graphics.getBackBuffer() };
	graphics.getDeviceContext()->OMSetRenderTargets(1, views, 0);

	// bind index buffer
	graphics.getDeviceContext()->IASetIndexBuffer(indexBuffers[1], DXGI_FORMAT_R32_UINT, 0);

	// use correct topology
	graphics.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// bind input layout
	graphics.getDeviceContext()->IASetInputLayout(inputLayouts[2]);

	// bind vertex shader
	graphics.getDeviceContext()->VSSetShader(vertexShaders[2], 0, 0);

	// bind pixel shader
	graphics.getDeviceContext()->PSSetShader(pixelShaders[2], 0, 0);

	// bind pixel shader textures
	graphics.getDeviceContext()->PSSetShaderResources(7, 1, renderResources);

	// disable depth testing
	graphics.getDeviceContext()->OMSetDepthStencilState(depthStates[0], 0);

	// draw to screen
	graphics.getDeviceContext()->DrawIndexed(numPlaneIndices, planeStartIndex, 0);

	// unbind render targets from resource slots
	ID3D11ShaderResourceView* unbind[1]{};
	graphics.getDeviceContext()->PSSetShaderResources(7, 1, unbind);
}