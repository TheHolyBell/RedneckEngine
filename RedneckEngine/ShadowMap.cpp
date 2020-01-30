#include "ShadowMap.h"
#include "Graphics.h"
#include "GraphicsThrowMacros.h"
#include "ConsoleClass.h"

ShadowMap::ShadowMap(Graphics& gfx, int width, int height, int slot)
	: m_Width(width), m_Height(height), m_Slot(slot)
{

	auto pDevice = GetDevice(gfx);
	auto pContext = GetContext(gfx);

	D3D11_TEXTURE2D_DESC colorDesc = {};
	colorDesc.Width = width;
	colorDesc.Height = height;
	colorDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	colorDesc.MipLevels = 1;
	colorDesc.ArraySize = 1;
	colorDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	colorDesc.SampleDesc.Count = 1;
	colorDesc.SampleDesc.Quality = 0;
	colorDesc.Usage = D3D11_USAGE_DEFAULT;
	
	if (FAILED(pDevice->CreateTexture2D(&colorDesc, nullptr, &m_DummyColor)))
		Console::WriteLine("%d %s", __LINE__, __FILE__);
	if (FAILED(pDevice->CreateRenderTargetView(m_DummyColor.Get(), nullptr, &m_DummyRTV)))
		Console::WriteLine("%d %s", __LINE__, __FILE__);

	

	D3D11_TEXTURE2D_DESC depthBufferDesc = {};
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.MipLevels = 1;
	
	if (FAILED(pDevice->CreateTexture2D(&depthBufferDesc, nullptr, &m_DepthBuffer)))
		Console::WriteLine("%d %s", __LINE__, __FILE__);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	
	if (FAILED(pDevice->CreateDepthStencilView(m_DepthBuffer.Get(), &dsvDesc, &m_DSV)))
		Console::WriteLine("%d %s", __LINE__, __FILE__);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;

	if (FAILED(pDevice->CreateShaderResourceView(m_DepthBuffer.Get(), &srvDesc, &m_SRV)))
		Console::WriteLine("%d %s", __LINE__, __FILE__);

	m_UID = "ShadowMap";
}

void ShadowMap::Bind(Graphics& gfx) noexcept
{
	D3D11_VIEWPORT vp = {};
	vp.Width = m_Width;
	vp.Height = m_Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	
	auto pDevice = GetDevice(gfx);
	auto pContext = GetContext(gfx);

	pContext->RSSetViewports(1, &vp);

	static float color[] = { 0,0,0,0 };
	pContext->ClearRenderTargetView(m_DummyRTV.Get(), color);
	pContext->ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	pContext->OMSetRenderTargets(1, m_DummyRTV.GetAddressOf(), m_DSV.Get());
}

std::string ShadowMap::GetUID() const noexcept
{
	return m_UID;
}

void ShadowMap::BindSRV(Graphics& gfx)
{
	GetContext(gfx)->PSSetShaderResources(m_Slot, 1, m_SRV.GetAddressOf());
}

void ShadowMap::Set(DirectX::XMFLOAT3 pos)
{
	using namespace DirectX;
	auto vPos = XMVectorSet(pos.x, pos.y, pos.z, 1.0f);
	auto vUp = XMVectorSet(0, 1, 0, 0);
	m_ViewMatrix = XMMatrixLookAtLH(vPos, XMVectorZero(), vUp);
}

ID3D11ShaderResourceView* ShadowMap::GetSRV()
{
	return m_SRV.Get();
}

ID3D11ShaderResourceView** ShadowMap::GetAddrSRV()
{
	return m_SRV.GetAddressOf();
}

DirectX::XMMATRIX ShadowMap::GetViewMatrix()
{
	return m_ViewMatrix;
}
