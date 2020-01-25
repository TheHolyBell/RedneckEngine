#pragma once
#include "Drawable.h"
#include "Entity.h"
#include "Texture3D.h"

class Graphics;

class Cubemap : public Entity, public Drawable
{
public:
	Cubemap(Graphics& gfx, const std::string& _cubemap_path);

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept override;

	virtual void Draw(Graphics& gfx) const noexcept(!IS_DEBUG) override;
	virtual bool IsMenuDrawable() const noexcept override;
	virtual void DrawMenu(Graphics& gfx) noexcept override;
	virtual void ItemSelected() noexcept override;

	void SetTransform(DirectX::XMMATRIX Transform);

	virtual std::string GetUID() const noexcept override;
private:
	std::string m_UID = "";
	bool m_bMenu = false;
	DirectX::XMMATRIX m_Transform;
	std::shared_ptr<Texture3D> m_Texture;
};