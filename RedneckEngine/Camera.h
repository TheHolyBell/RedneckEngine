#pragma once
#include <DirectXMath.h>

class Camera
{
public:
	Camera() noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
	DirectX::XMMATRIX GetView() const noexcept;
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Rotate(float dx, float dy) noexcept;
	void Translate(DirectX::XMFLOAT3 translation) noexcept;
	DirectX::XMFLOAT3 GetPos() const noexcept;
	void GenerateProjection(int width, int height, float fov);
private:
	DirectX::XMFLOAT3 pos = {};
	float pitch;
	float yaw;
	static float travelSpeed;
	DirectX::XMMATRIX m_projection;
	static constexpr float rotationSpeed = 0.004f;
};