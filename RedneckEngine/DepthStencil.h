#pragma once
#include "IBindable.h"

class Graphics;

namespace Bind
{
	class DepthStencil : public IBindable
	{
	public:
		enum class Func
		{
			NEVER = 1,
			LESS = 2,
			EQUAL = 3,
			LEQUAL = 4,
			GREATER = 5,
			NEQUAL = 6,
			GEQUAL = 7,
			ALWAYS = 8
		};
		enum class StencilMode
		{
			Off,
			Write,
			Mask
		};
	public:
		DepthStencil(Graphics& gfx, Func depthFunc = Func::LESS, bool depthEnable = true, Func stencilFunc = Func::NEVER, StencilMode stencilMode = StencilMode::Off);

		virtual void Bind(Graphics& gfx) noexcept override;
		virtual std::string GetUID() const noexcept override;

		static std::shared_ptr<DepthStencil> Resolve(Graphics& gfx, Func depthFunc = Func::LESS, bool depthEnable = true, Func stencilFunc = Func::NEVER, StencilMode stencilMode = StencilMode::Off);
		static std::string GenerateUID(Func depthFunc, bool depthEnable, Func stencilFunc, StencilMode stencilMode);

	private:
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pState;
		Func m_depthFunc;
		Func m_stencilFunc;
		StencilMode m_stencilMode;
		bool m_depth = false;
	};
}