#pragma once
#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffers.h"
#include "Entity.h"

class PointLight : public Entity
{
public:
	PointLight(Graphics& gfx, float radius = 0.5f);
	
	virtual void Update(float dt) override;

	virtual bool IsMenuDrawable() const noexcept override;
	virtual void DrawMenu(Graphics& gfx) noexcept override;
	virtual void ItemSelected() noexcept override;

	virtual std::string GetUID() const noexcept override;

	virtual void Draw(Graphics& gfx) const noexcept(!IS_DEBUG) override;

	DirectX::XMFLOAT3 GetPos() const noexcept;

private:
	struct PointLightCBuf
	{
		alignas(16) DirectX::XMFLOAT3 pos;
		alignas(16) DirectX::XMFLOAT3 ambient;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};

private:
	PointLightCBuf m_cbData = {};
	mutable SolidSphere m_mesh;
	mutable Bind::PixelConstantBuffer<PointLightCBuf> m_cbuf;
	bool m_bMenu = false;
};