#pragma once

#include <DirectXMath.h>

struct Camera {
	DirectX::XMFLOAT3 eye{ 1.0f, 0.0f, 0.0f }, focus{}, up{ 0.0f, 0.0f, 1.0f };
	DirectX::XMMATRIX viewTransform[2], projTransform[2];
	Camera();
	void updateViewTransform(), updateProjTransform();
};