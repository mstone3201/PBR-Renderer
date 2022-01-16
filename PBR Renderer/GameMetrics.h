#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

struct GameMetrics {
	LPCWSTR gameName = L"PBR Renderer";
	unsigned width = 1920, height = 1080;
	bool fullScreen = false;
	float fov = DirectX::XMConvertToRadians(50.0f), screenNear = 0.01f, screenFar = 50.0f,
		mouseSensitivity = 1.5f;
	UINT anisotropy = D3D11_MAX_MAXANISOTROPY;
	GameMetrics();
};

extern GameMetrics metrics;