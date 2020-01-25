#pragma once
#include "Drawable.h"
#include "Entity.h"
#include "IMenuViewable.h"

class TestCylinder : public Entity, public Drawable
{
public:
	TestCylinder(Graphics& gfx, float bottomRadius, float topRadius, float height, UINT sliceCount = 30, UINT stackCount = 30);
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	void SetRotation(float pitch, float yaw, float roll) noexcept;

	virtual void Draw(Graphics& gfx) const noexcept(!IS_DEBUG) override;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept override;

	virtual std::string GetUID() const noexcept override;

	virtual void ItemSelected() noexcept override;
	virtual bool IsMenuDrawable() const noexcept override;
	virtual void DrawMenu(Graphics& gfx) noexcept override;
private:

	DirectX::XMFLOAT3 m_pos = { 1.0f, 1.0f, 1.0f };
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;

	std::string m_UID = "";
	bool m_bMenu = false;
};