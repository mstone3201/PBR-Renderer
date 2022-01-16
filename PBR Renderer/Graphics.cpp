#include "Graphics.h"
#include "GameMetrics.h"

Graphics::Graphics() {}

Graphics::~Graphics() {
	if(swapChain) swapChain->Release();
	if(device) device->Release();
	if(deviceContext) deviceContext->Release();
	if(backBuffer) backBuffer->Release();
}

HRESULT Graphics::init(HWND hwnd) {
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferDesc.Width = metrics.width;
	swapChainDesc.BufferDesc.Height = metrics.height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	HRESULT result = D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_DEBUG, 0, 0, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, 0, &deviceContext);
	if(FAILED(result)) return result;

	return resize();
}

HRESULT Graphics::resize() {
	if(backBuffer) {
		deviceContext->OMSetRenderTargets(0, 0, 0);
		backBuffer->Release();
	}
	HRESULT result = swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	if(FAILED(result)) return result;

	ID3D11Texture2D* backBufferTexture = 0;
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
	if(FAILED(result)) return result;

	D3D11_TEXTURE2D_DESC backBufferTextureDesc;
	backBufferTexture->GetDesc(&backBufferTextureDesc);
	result = device->CreateRenderTargetView(backBufferTexture, 0, &backBuffer);
	backBufferTexture->Release(); // release texture even in the case of RTV creation failure

	viewPort.Width = (float)backBufferTextureDesc.Width;
	viewPort.Height = (float)backBufferTextureDesc.Height;

	return result;
}

void Graphics::setFullScreen() {
	swapChain->SetFullscreenState(metrics.fullScreen, 0);
}

void Graphics::setFullScreen(bool full) {
	swapChain->SetFullscreenState(full, 0);
}

void Graphics::render() {
	swapChain->Present(0, 0);
}

IDXGISwapChain* Graphics::getSwapChain() {
	return swapChain;
}

ID3D11Device* Graphics::getDevice() {
	return device;
}

ID3D11DeviceContext* Graphics::getDeviceContext() {
	return deviceContext;
}

ID3D11RenderTargetView* Graphics::getBackBuffer() {
	return backBuffer;
}