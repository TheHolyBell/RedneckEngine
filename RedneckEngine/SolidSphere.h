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

	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
private:
	DirectX::XMFLOAT3 m_pos = { 1.0f, 1.0f, 1.0f };
	std::string m_name;
};