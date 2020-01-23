#pragma once
#include "IBindable.h"
#include "Vertex.h"

namespace Bind
{
	class InputLayout : public IBindable
	{
	public:
		InputLayout(Graphics& gfx,
			Dvtx::VertexLayout layout,
			ID3DBlob* pVertexShaderBytecode);

		virtual void Bind(Graphics& gfx) noexcept override;

		static std::shared_ptr<InputLayout> Resolve(Graphics& gfx,
			const Dvtx::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode);
		static std::string GenerateUID(const Dvtx::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode = nullptr);
		virtual std::string GetUID() const noexcept override;
	protected:
		Dvtx::VertexLayout m_layout;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	};
}