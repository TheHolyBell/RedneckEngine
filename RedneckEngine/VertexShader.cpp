#include "VertexShader.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"

namespace Bind
{
	using namespace std::string_literals;
	VertexShader::VertexShader(Graphics& gfx, const std::wstring& path)
		: VertexShader(gfx, std::string(path.begin(), path.end()))
	{
	}

	VertexShader::VertexShader(Graphics& gfx, const std::string& path)
		: m_path(path)
	{
		INFOMAN(gfx);

		std::wstring _widepath(path.begin(), path.end());

		GFX_THROW_INFO(D3DReadFileToBlob(_widepath.c_str(), &m_pBytecodeBlob));
		GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(
			m_pBytecodeBlob->GetBufferPointer(),
			m_pBytecodeBlob->GetBufferSize(),
			nullptr,
			&m_pVertexShader
		));
	}

	void VertexShader::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	}

	ID3DBlob* VertexShader::GetBytecode() const noexcept
	{
		return m_pBytecodeBlob.Get();
	}
	std::shared_ptr<VertexShader> VertexShader::Resolve(Graphics& gfx, const std::string& path)
	{
		return Codex::Resolve<VertexShader>(gfx, path);
	}
	std::string VertexShader::GenerateUID(const std::string& path)
	{
		return typeid(VertexShader).name() + "#"s + path;
	}
	std::string VertexShader::GetUID() const noexcept
	{
		return GenerateUID(m_path);
	}
}
