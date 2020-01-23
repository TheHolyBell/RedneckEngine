#pragma once
#include "IBindable.h"

namespace Bind
{
	class Topology : public IBindable
	{
	public:
		Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
		virtual void Bind(Graphics& gfx) noexcept override;

		static std::shared_ptr<Topology> Resolve(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
		static std::string GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type);
		virtual std::string GetUID() const noexcept override;
	protected:
		D3D11_PRIMITIVE_TOPOLOGY m_type;
	};
}