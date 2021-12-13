#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d11.h>

class Graphics {
	ID3D11Device* device = 0;
	ID3D11DeviceContext* deviceContext = 0;
	IDXGISwapChain* swapChain = 0;
	ID3D11RenderTargetView* backBuffer = 0;
	D3D11_VIEWPORT viewPort;
public:
	Graphics();
	~Graphics();
	HRESULT init(HWND), resize();
	void setFullScreen(), setFullScreen(bool), prepareTarget(), render();
	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
};