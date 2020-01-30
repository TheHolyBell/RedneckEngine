#pragma once
#include "Entity.h"
#include "Drawable.h"
#include "PBRCommon.h"

using namespace Bind;
class PBRPlane : public Entity, public Drawable
{
public:
	PBRPlane(Graphics& gfx, int width, int depth, PBRInitInfo info);

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
		DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}pmc;
	DirectX::XMFLOAT3 m_pos = { 1.0f, 1.0f, 1.0f };
	std::string m_UID;
	bool m_bMenu = false;
};