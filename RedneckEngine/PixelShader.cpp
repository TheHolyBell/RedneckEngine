#include "PixelShader.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"
#include <d3dcompiler.h>

namespace Bind
{
	PixelShader::PixelShader(Graphics& gfx, const std::wstring& path)
		: PixelShader(gfx, std::string(path.begin(), path.end()))
	{}

	PixelShader::PixelShader(Graphics& gfx, const std::string& path)
		: m_path(path)
	{
		INFOMAN(gfx);

		auto _widepath = std::wstring(path.begin(), path.end());

		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		GFX_THROW_INFO(D3DReadFileToBlob(_widepath.c_str(), &pBlob));
		GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pPixelShader));
	}

	void PixelShader::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
	}
	std::shared_ptr<PixelShader> PixelShader::Resolve(Graphics& gfx, const std::string& path)
	{
		return Codex::Resolve<PixelShader>(gfx, path);
	}
	std::string PixelShader::GenerateUID(const std::string& path)
	{
		using namespace std::string_literals;
		return typeid(PixelShader).name() + "#"s + path;
	}
	std::string PixelShader::GetUID() const noexcept
	{
		return GenerateUID(m_path);
	}
}