#define _CRT_SECURE_NO_WARNINGS
#include "Graphics.h"
#include "GraphicsThrowMacros.h"
#include <sstream>
#include <d3dcompiler.h>
#include "ConsoleClass.h"
#include "dxerr.h"
#include "Event.h"
#include "WindowEvents.h"

#include "ImGui\imgui_impl_dx11.h"
#include "ImGui\imgui_impl_win32.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

Graphics::Graphics(Window* pWnd)
{
	InitializeDirectX(pWnd);

	m_camera.GenerateProjection(pWnd->GetWidth(), pWnd->GetHeight(), DirectX::XM_PIDIV2);

	m_camera.Translate(DirectX::XMFLOAT3{ 0.0f, 0.0f, -0.5f });

	// Register callback function(s) for appropriate handling resizing (i.e. for changing dimensions of buffers etc.)
	pWnd->OnResizeHandler += [](int width, int height) {Console::WriteLine("Current window dimensions: (%d;%d)", width, height); };
	pWnd->OnResizeHandler += [&](int width, int height) {ResizeBuffers(width, height); };
	pWnd->OnResizeHandler += [&](int width, int height) {m_camera.GenerateProjection(width, height, DirectX::XM_PIDIV2); };
	EventDispatcher::AddGlobalEventListener<WindowResizeEvent>(*this);
}

void Graphics::OnEvent(const WindowResizeEvent& event)
{
	Console::WriteLine("%d %d occurred", event.width, event.height);
}

void Graphics::BeginFrame(float red, float green, float blue, float alpha)
{
	float color[] = {red, green, blue, alpha};
	BeginFrame(color);
}

void Graphics::BeginFrame(float color[4])
{
	if (m_bImGuiEnabled)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
	m_pDeviceContext->ClearRenderTargetView(m_pTarget.Get(), color);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0);
}

void Graphics::EndFrame()
{
	if (m_bImGuiEnabled)
	{
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	HRESULT hr;
#ifndef NDEBUG
	infoManager.Set();
#endif
	if (FAILED(hr = m_pSwapChain->Present(1, 0)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
			throw GFX_DEVICE_REMOVED_EXCEPT(m_pDevice->GetDeviceRemovedReason());
		else
			throw GFX_EXCEPT(hr);
	}
}

void Graphics::DrawIndexed(UINT count) noexcept(!IS_DEBUG)
{
	GFX_THROW_INFO_ONLY(m_pDeviceContext->DrawIndexed(count, 0, 0));
}

void Graphics::EnableImGui() noexcept
{
	m_bImGuiEnabled = true;
}

void Graphics::DisableImGui() noexcept
{
	m_bImGuiEnabled = false;
}

bool Graphics::IsImGuiEnabled() const noexcept
{
	return m_bImGuiEnabled;
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept
{
	return m_camera.GetProjection();
}

DirectX::XMMATRIX Graphics::GetView() const noexcept
{
	return m_camera.GetView();
}

void Graphics::InitializeDirectX(Window* pWnd)
{
	int width = pWnd->GetWidth();
	int height = pWnd->GetHeight();

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 4;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = pWnd->GetHandle();
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// for checking results of d3d functions
	HRESULT hr;

	// create device and front/back buffers, and swap chain and rendering context
	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&m_pSwapChain,
		&m_pDevice,
		nullptr,
		&m_pDeviceContext
	));

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//GFX_THROW_INFO(m_pDevice->CreateDepthStencilState(&dsDesc, &m_pDSState));

	// Bind state
	//m_pContext->OMSetDepthStencilState(m_pDSState.Get(), 0);


	ResizeBuffers(width, height);

	ImGui_ImplDX11_Init(m_pDevice.Get(), m_pDeviceContext.Get());
}

void Graphics::ResizeBuffers(int width, int height)
{
	if (m_pSwapChain == nullptr)
		return;

	HRESULT hr;

	m_pTarget.Reset();
	m_pDepthStencilView.Reset();
	m_pDepthBuffer.Reset();

	m_pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_B8G8R8A8_UNORM, 0);

	ComPtr<ID3D11Texture2D> swapChainBufferReference;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &swapChainBufferReference);
	m_pDevice->CreateRenderTargetView(swapChainBufferReference.Get(), nullptr, &m_pTarget);

	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.SampleDesc.Count = 4;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

	m_pDevice->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthBuffer);
	m_pDevice->CreateDepthStencilView(m_pDepthBuffer.Get(), nullptr, &m_pDepthStencilView);

	m_pDeviceContext->OMSetRenderTargets(1, m_pTarget.GetAddressOf(), m_pDepthStencilView.Get());


	D3D11_VIEWPORT vp = {};

	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.Width = width;
	vp.Height = height;

	m_pDeviceContext->RSSetViewports(1, &vp);
}


#pragma region Graphics_Exceptions
Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line, file),
	hr(hr)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	if (!info.empty())
	{
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
	return "Redneck Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
	return DXGetErrorString(hr);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescription(hr, buf, sizeof(buf));
	return buf;
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
	return info;
}


const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Redneck Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}
Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line, file)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}


const char* Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
	return "Redneck Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
	return info;
}

#pragma endregion