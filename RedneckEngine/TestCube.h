#pragma once
#include "Entity.h"
#include "Drawable.h"
#include "IMenuViewable.h"

class TestCube : public Entity, public Drawable
{
public:
	TestCube(Graphics& gfx, float size);
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	void SetRotation(float pitch, float yaw, float roll) noexcept;

	virtual void Draw(Graphics& gfx) const noexcept(!IS_DEBUG) override;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept override;
	
	virtual std::string GetUID() const noexcept override;
	
	virtual void ItemSelected() noexcept override;
	virtual bool IsMenuDrawable() const noexcept override;
	virtual void DrawMenu(Graphics& gfx) noexcept override;
private:
	struct PSMaterialConstant
	{
		float specularIntensity = 0.1f;
		float specularPower = 20.0f;
		alignas(4) bool normalMappingEnabled = true;
		float padding;
	} pmc;
	DirectX::XMFLOAT3 m_pos = { 1.0f, 1.0f, 1.0f };
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;

	std::string m_UID = "";
	bool m_bMenu = false;
};