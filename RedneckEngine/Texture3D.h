#pragma once
#include "IBindable.h"
#include <string>

class Graphics;

namespace Bind
{
	class Texture3D : public IBindable
	{
	public:
		Texture3D(Graphics& gfx, const std::string& _texture_path, UINT slot = 0);

		virtual void Bind(Graphics& gfx) noexcept override;

		static std::shared_ptr<Texture3D> Resolve(Graphics& gfx, const std::string& path, UINT slot = 0);
		static std::string GenerateUID(const std::string& path, UINT slot = 0);
		virtual std::string GetUID() const noexcept override;

	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTexture;
		std::string m_UID = "";
		UINT m_slot = 0;
	};
}