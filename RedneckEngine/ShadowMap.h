#pragma once
#include "IBindable.h"

class Graphics;

class ShadowMap : public Bind::IBindable
{
public:
	ShadowMap(Graphics& gfx, int width, int height, int slot = 5);
	virtual void Bind(Graphics& gfx) noexcept override;
	virtual std::string GetUID() const noexcept override;

	void BindSRV(Graphics& gfx);

	void Set(DirectX::XMFLOAT3 pos);

	ID3D11ShaderResourceView* GetSRV();
	ID3D11ShaderResourceView** GetAddrSRV();

	DirectX::XMMATRIX GetViewMatrix();

private:
	std::string m_UID;
	int m_Width;
	int m_Height;
	int m_Slot;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_DummyColor;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_DummyRTV;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_DepthBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DSV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;

	DirectX::XMMATRIX m_ViewMatrix;
};