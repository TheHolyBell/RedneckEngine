#pragma once

#include <d3d11.h>
#include "DXGIInfoManager.h"
#include "WindowClass.h"
#include "RedneckException.h"
#include <DirectXMath.h>
#include "Camera.h"

namespace Bind
{
	class IBindable;
}

class Graphics
{
	friend class Bind::IBindable;
public:
	class Exception : public RedneckException
	{
		using RedneckException::RedneckException;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string info;
	};
	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;

		virtual const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		std::string GetErrorInfo()const noexcept;
	private:
		std::string info;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	private:
		std::string reason;
	};
public:
	Graphics(Window* pWnd);

	Graphics(const Graphics& rhs) = delete;
	Graphics& operator=(const Graphics& rhs) = delete;

	~Graphics() = default;

	void BeginFrame(float red, float green, float blue, float alpha);
	void BeginFrame(float color[4]);
	void EndFrame();
	void DrawIndexed(UINT count) noexcept(!IS_DEBUG);

	DirectX::XMMATRIX GetProjection() const noexcept;
	DirectX::XMMATRIX GetView() const noexcept;

private:
	void InitializeDirectX(Window* pWnd);

	void ResizeBuffers(int width, int height);

#ifndef NDEBUG
	DXGIInfoManager infoManager;
#endif

	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pTarget;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pDepthBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;

	Camera m_camera;
};