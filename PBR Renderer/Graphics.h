#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d11.h>

class Graphics {
	IDXGISwapChain* swapChain = 0;
	ID3D11Device* device = 0;
	ID3D11DeviceContext* deviceContext = 0;
	ID3D11RenderTargetView* backBuffer = 0;
public:
	D3D11_VIEWPORT viewPort{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	Graphics();
	~Graphics();
	HRESULT init(HWND), resize();
	void setFullScreen(), setFullScreen(bool), render();
	IDXGISwapChain* getSwapChain();
	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
	ID3D11RenderTargetView* getBackBuffer();
};