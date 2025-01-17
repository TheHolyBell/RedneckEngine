#pragma once
#include "IBindable.h"
#include "GraphicsThrowMacros.h"
#include "Vertex.h"

namespace Bind
{
	class VertexBuffer : public IBindable
	{
	public:
		VertexBuffer(Graphics& gfx, const std::string& tag, const Dvtx::VertexBuffer& vbuf);
		VertexBuffer(Graphics& gfx, const Dvtx::VertexBuffer& vbuf);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<VertexBuffer> Resolve(Graphics& gfx, const std::string& tag,
			const Dvtx::VertexBuffer& vbuf);
		template<typename...Ignore>
		static std::string GenerateUID(const std::string& tag, Ignore&&...ignore)
		{
			return GenerateUID_(tag);
		}
		virtual std::string GetUID() const noexcept override;
	private:
		static std::string GenerateUID_(const std::string& tag);
	protected:
		UINT stride;
		std::string m_tag;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
	};
}