#pragma once
#include "IBindable.h"

namespace Bind
{
	class IndexBuffer : public IBindable
	{
	public:
		IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices);
		IndexBuffer(Graphics& gfx, std::string tag, const std::vector<unsigned short>& indices);
	
		virtual void Bind(Graphics& gfx) noexcept override;
		UINT GetCount() const noexcept;

		static std::shared_ptr<IndexBuffer> Resolve(Graphics& gfx, const std::string& tag,
			const std::vector<unsigned short>& indices);
		template<typename... Ignore>
		static std::string GenerateUID(const std::string& tag, Ignore&&...ignore)
		{
			return GenerateUID_(tag);
		}
		virtual std::string GetUID() const noexcept override;
	private:
		static std::string GenerateUID_(const std::string& tag);
	protected:
		UINT m_count = 0;
		std::string m_tag;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
	};
}