#include "Texture.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#include "RedneckUtility.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"
#include "ConsoleClass.h"

namespace Bind
{
	namespace wrl = Microsoft::WRL;

	Texture::Texture(Graphics& gfx, const std::string& path, UINT slot)
		:
		m_path(path),
		slot(slot)
	{
		INFOMAN(gfx);

		auto pDevice = GetDevice(gfx);
		auto pContext = GetContext(gfx);

		auto file_extension = StringHelper::GetFileExtension(path);
		DirectX::DDS_ALPHA_MODE alpha = {};
		if (file_extension == "dds")
			DirectX::CreateDDSTextureFromFile(pDevice, pContext, StringHelper::ToWide(path).c_str(), nullptr, &m_pTextureView, 0Ui64, &alpha);
		else
			DirectX::CreateWICTextureFromFile(pDevice, pContext, StringHelper::ToWide(path).c_str(), nullptr, &m_pTextureView);

		m_bHasAlpha = alpha != DirectX::DDS_ALPHA_MODE_OPAQUE;
	}

	void Texture::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->PSSetShaderResources(slot, 1u, m_pTextureView.GetAddressOf());
	}
	std::shared_ptr<Texture> Texture::Resolve(Graphics& gfx, const std::string& path, UINT slot)
	{
		return Codex::Resolve<Texture>(gfx, path, slot);
	}
	std::string Texture::GenerateUID(const std::string& path, UINT slot)
	{
		using namespace std::string_literals;
		return typeid(Texture).name() + "#"s + path + "#" + std::to_string(slot);
	}
	std::string Texture::GetUID() const noexcept
	{
		return GenerateUID(m_path, slot);
	}
	bool Texture::HasAlpha() const noexcept
	{
		return m_bHasAlpha;
	}
}