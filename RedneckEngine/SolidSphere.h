#pragma once
#include "Entity.h"
#include "Drawable.h"

using namespace Bind;
class SolidSphere : public Entity, public Drawable
{
public:
	SolidSphere(Graphics& gfx, float radius);

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept override;

	virtual void Draw(Graphics& gfx) const noexcept(!IS_DEBUG) override;

	virtual std::string GetUID() const noexcept;

	virtual void SetPos(DirectX::XMFLOAT3 pos) noexcept;


	virtual bool IsMenuDrawable() const noexcept;
	virtual void DrawMenu(Graphics& gfx) noexcept;
	virtual void ItemSelected() noexcept;

protected:
	struct PSColorConstant
	{
		DirectX::XMFLOAT4 color = {1.0f, 0.0f, 0.0f, 1.0f};
	}pmc;
	DirectX::XMFLOAT3 m_pos = { 1.0f, 1.0f, 1.0f };
	std::string m_UID;
	bool m_bMenu = false;
};