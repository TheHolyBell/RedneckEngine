#include "Texture3D.h"
#include <filesystem>
#include "Graphics.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#include "RedneckUtility.h"
#include "ConsoleClass.h"


namespace Bind
{
	Texture3D::Texture3D(Graphics& gfx, const std::string& _texture_path, UINT slot)
		: m_UID(std::filesystem::absolute(_texture_path).string()), m_slot(slot)
	{
		INFOMAN(gfx);

		auto pDevice = GetDevice(gfx);
		auto pContext = GetContext(gfx);

		if (StringHelper::GetFileExtension(m_UID) == "dds")
			DirectX::CreateDDSTextureFromFile(pDevice, pContext, StringHelper::ToWide(m_UID).c_str(), nullptr, &m_pTexture);
		else
			DirectX::CreateWICTextureFromFile(pDevice, pContext, StringHelper::ToWide(m_UID).c_str(), nullptr, &m_pTexture);
	}
	void Texture3D::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->PSSetShaderResources(m_slot, 1, m_pTexture.GetAddressOf());
	}
	std::shared_ptr<Texture3D> Texture3D::Resolve(Graphics& gfx, const std::string& path, UINT slot)
	{
		return Codex::Resolve<Texture3D>(gfx, path, slot);
	}
	std::string Texture3D::GenerateUID(const std::string& path, UINT slot)
	{
		return path + "#" + std::to_string(slot);
	}
	std::string Texture3D::GetUID() const noexcept
	{
		return GenerateUID(m_UID, m_slot);
	}
}