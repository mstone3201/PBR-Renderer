#pragma once

#include "Camera.h"
#include "Input.h"

class CameraController : public Camera {
	float yaw = 0.0f, pitch = DirectX::XMConvertToRadians(35.0f),
		minPitch = DirectX::XMConvertToRadians(-89.9f), maxPitch = DirectX::XMConvertToRadians(89.9f),
		minArmLength = 0.3f, maxArmLength = 10.0f, armLength = (minArmLength + maxArmLength) * 0.5f,
		targetArmLength = armLength, scrollTime = 0.075f;
public:
	void update(float, Input&);
	float getZoomFactor(), getSmoothZoomFactor();
};