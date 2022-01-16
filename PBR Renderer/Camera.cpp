#include "Camera.h"
#include "GameMetrics.h"

Camera::Camera() {
	updateViewTransform();
	updateProjTransform();
}

void Camera::updateViewTransform() {
	viewTransform[0] = DirectX::XMMatrixLookAtRH(DirectX::XMLoadFloat3(&eye), DirectX::XMLoadFloat3(&focus), DirectX::XMLoadFloat3(&up));
	viewTransform[1] = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(0, viewTransform[0]));
	viewTransform[0] = DirectX::XMMatrixTranspose(viewTransform[0]);
}

void Camera::updateProjTransform() {
	projTransform[0] = DirectX::XMMatrixPerspectiveFovRH(metrics.fov, (float)metrics.width/metrics.height, metrics.screenNear, metrics.screenFar);
	projTransform[1] = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(0, projTransform[0]));
	projTransform[0] = DirectX::XMMatrixTranspose(projTransform[0]);
}