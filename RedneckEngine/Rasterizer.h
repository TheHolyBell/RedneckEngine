#pragma once
#include "IBindable.h"
#include <array>

namespace Bind
{
	class Rasterizer : public IBindable
	{
	public:
		Rasterizer(Graphics& gfx, bool twoSided);

		virtual void Bind(Graphics& gfx) noexcept override;

		static std::shared_ptr<Rasterizer> Resolve(Graphics& gfx, bool twoSided);
		static std::string GenerateUID(bool twoSided);

		virtual std::string GetUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pRasterizer;
		bool m_twoSided;
	};
}