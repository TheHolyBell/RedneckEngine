#pragma once

#include "Entity.h"
#include "IMenuViewable.h"

class TestPlane : public Entity, public IMenuViewable
{
public:
	TestPlane(Graphics& gfx, float size, DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f,1.0f });

	virtual std::string GetName() const noexcept override;
	virtual bool IsMenuDrawable() const noexcept override;
	virtual void DrawMenu(Graphics& gfx) noexcept override;
	virtual void ItemSelected() noexcept override;

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept override;
	virtual std::string GetUID() const noexcept override;

private:
	struct PSMaterialConstant
	{
		DirectX::XMFLOAT4 color;
	}pmc;
	DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;

	std::string m_UID = "";
	bool m_bMenu = false;
};