#pragma once
#include <DirectXMath.h>
#include "MouseEvents.h"
#include "KeyboardEvents.h"
#include "IUpdateable.h"
#include <memory>

namespace Bind
{
	template<typename C>
	class PixelConstantBuffer;
}

class Graphics;

class Camera : public IUpdateable
{
public:
	Camera() noexcept;

	void Initialize(Graphics& gfx);

	DirectX::XMMATRIX GetProjection() const noexcept;
	DirectX::XMMATRIX GetView() const noexcept;
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Rotate(float dx, float dy) noexcept;
	void Translate(DirectX::XMFLOAT3 translation) noexcept;
	DirectX::XMFLOAT3 GetPos() const noexcept;
	void GenerateProjection(int width, int height, float fov);

	virtual void Update(float dt) override;

	DirectX::XMVECTOR GetLookVector() const noexcept;

	void Bind(Graphics& gfx);

	void OnEvent(const MouseMovedEvent& event);
private:
	DirectX::XMFLOAT3 pos = {};
	float pitch;
	float yaw;
	static float travelSpeed;
	float dt = 0;
	DirectX::XMMATRIX m_projection;
	static constexpr float rotationSpeed = 0.004f;

	struct PSConstantBuffer
	{
		DirectX::XMFLOAT3 cameraPos;
		float padding;
	} pmc;
	std::shared_ptr<Bind::PixelConstantBuffer<PSConstantBuffer>> m_cbuf;
};