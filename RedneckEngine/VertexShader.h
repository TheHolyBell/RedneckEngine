#pragma once
#include "IBindable.h"

namespace Bind
{
	class VertexShader : public IBindable
	{
	public:
		VertexShader(Graphics& gfx, const std::wstring& path);
		VertexShader(Graphics& gfx, const std::string& path);

		virtual void Bind(Graphics& gfx) noexcept override;
		ID3DBlob* GetBytecode() const noexcept;

		static std::shared_ptr<VertexShader> Resolve(Graphics& gfx, const std::string& path);
		static std::string GenerateUID(const std::string& path);
		virtual std::string GetUID() const noexcept override;
	protected:
		std::string m_path;
		Microsoft::WRL::ComPtr<ID3DBlob> m_pBytecodeBlob;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	};
}