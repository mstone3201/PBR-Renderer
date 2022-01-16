#include "CameraController.h"

void CameraController::update(float elapsed, Input& input) {
	DirectX::XMFLOAT2 velocity{};

	if(input.isInput(KEYBIND::INPUT_ROTATE)) {
		yaw = fmodf(yaw - input.getDX(), DirectX::XM_2PI);
		pitch = min(max(pitch + input.getDY(), minPitch), maxPitch);
	} else if(input.isInput(KEYBIND::INPUT_PAN))
		velocity = { input.getDX() * 4.0f * getSmoothZoomFactor(), input.getDY() * 4.0f * getSmoothZoomFactor() };

	if(input.getScroll()) targetArmLength = min(max(targetArmLength - input.getScroll() * 2.0f * getZoomFactor(), minArmLength), maxArmLength);
	armLength += (targetArmLength - armLength) * elapsed / scrollTime;

	DirectX::XMFLOAT2 yawaim(cosf(yaw), sinf(yaw));
	float xylen = cosf(pitch);
	DirectX::XMFLOAT3 aim(xylen * yawaim.x, xylen * yawaim.y, sinf(pitch)); // sign is flipped
	DirectX::XMFLOAT3 left(yawaim.y, -yawaim.x, 0.0f); // left = normalize(aim x up)
	DirectX::XMFLOAT3 up; // up = left x aim
	DirectX::XMStoreFloat3(&up, DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&left), DirectX::XMLoadFloat3(&aim)));

	focus = { focus.x + left.x * velocity.x + up.x * velocity.y, focus.y + left.y * velocity.x + up.y * velocity.y, focus.z + left.z * velocity.x + up.z * velocity.y };

	eye = { focus.x + armLength * aim.x, focus.y + armLength * aim.y, focus.z + armLength * aim.z };

	updateViewTransform();
}

float CameraController::getZoomFactor() {
	return targetArmLength/maxArmLength;
}

float CameraController::getSmoothZoomFactor() {
	return armLength/maxArmLength;
}