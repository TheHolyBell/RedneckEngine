#pragma once
#include "IBindable.h"

namespace Bind
{
	class Texture : public IBindable
	{
	public:
		Texture(Graphics& gfx, const std::string& path, UINT slot = 0);
	
		virtual void Bind(Graphics& gfx) noexcept override;

		static std::shared_ptr<Texture> Resolve(Graphics& gfx, const std::string& path, UINT slot = 0);
		static std::string GenerateUID(const std::string& path, UINT slot = 0);
		virtual std::string GetUID() const noexcept override;
		bool HasAlpha() const noexcept;

	private:		
		unsigned int slot = 0;
	protected:
		bool m_bHasAlpha = false;
		std::string m_path;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureView;
	};
}