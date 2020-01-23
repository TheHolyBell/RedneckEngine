#pragma once
#include "IBindable.h"

namespace Bind
{
	class Sampler : public IBindable
	{
	public:
		Sampler(Graphics& gfx);

		virtual void Bind(Graphics& gfx) noexcept override;

		static std::shared_ptr<Sampler> Resolve(Graphics& gfx);
		static std::string GenerateUID();

		virtual std::string GetUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampler;
	};
}