#pragma once
#include "IBindable.h"

namespace Bind
{
	class PixelShader : public IBindable
	{
	public:
		PixelShader(Graphics& gfx, const std::wstring& path);
		PixelShader(Graphics& gfx, const std::string& path);
		virtual void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<PixelShader> Resolve(Graphics& gfx, const std::string& path);
		static std::string GenerateUID(const std::string& path);
		virtual std::string GetUID() const noexcept override;
	protected:
		std::string m_path;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	};
}