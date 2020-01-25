#include "DepthStencil.h"
#include "Graphics.h"
#include "BindableCodex.h"
#include "GraphicsThrowMacros.h"
#include "ConsoleClass.h"

static D3D11_COMPARISON_FUNC GetD3D11ComparisonFunc(Bind::DepthStencil::Func func)
{
	using Func = Bind::DepthStencil::Func;

	switch (func)
	{
	case Func::NEVER:
		return D3D11_COMPARISON_NEVER;
	case Func::LESS:
		return D3D11_COMPARISON_LESS;
	case Func::EQUAL:
		return D3D11_COMPARISON_EQUAL;
	case Func::LEQUAL:
		return D3D11_COMPARISON_LESS_EQUAL;
	case Func::GREATER:
		return D3D11_COMPARISON_GREATER;
	case Func::NEQUAL:
		return D3D11_COMPARISON_NOT_EQUAL;
	case Func::GEQUAL:
		return D3D11_COMPARISON_GREATER_EQUAL;
	case Func::ALWAYS:
		return D3D11_COMPARISON_ALWAYS;
	}
}

static const std::string ConvertFuncToString(Bind::DepthStencil::Func func)
{
	using Func = Bind::DepthStencil::Func;

	switch (func)
	{
	case Func::NEVER:
		return "FUNC_NEVER";
	case Func::LESS:
		return "FUNC_LESS";
	case Func::EQUAL:
		return "FUNC_EQUAL";
	case Func::LEQUAL:
		return "FUNC_LEQUAL";
	case Func::GREATER:
		return "FUNC_GREATER";
	case Func::NEQUAL:
		return "FUNC_NEQUAL";
	case Func::GEQUAL:
		return "FUNC_GEQUAL";
	case Func::ALWAYS:
		return "FUNC_ALWAYS";
	}
}

static const std::string ConvertModeToString(Bind::DepthStencil::StencilMode mode)
{
	using Mode = Bind::DepthStencil::StencilMode;

	switch (mode)
	{
	case Mode::Off:
		return "Off";
	case Mode::Mask:
		return "Mask";
	case Mode::Write:
		return "Write";
	}
}

namespace Bind
{
	DepthStencil::DepthStencil(Graphics& gfx, Func depthFunc, bool depthEnable, Func stencilFunc, StencilMode stencilMode)
		: m_depthFunc(depthFunc), m_stencilFunc(stencilFunc), m_stencilMode(stencilMode), m_depth(depthEnable)
	{
		INFOMAN(gfx);

		D3D11_DEPTH_STENCIL_DESC desc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT{});

		desc.DepthFunc = GetD3D11ComparisonFunc(depthFunc);
		desc.FrontFace.StencilFunc = GetD3D11ComparisonFunc(stencilFunc);
		desc.DepthEnable = m_depth;

		if (stencilMode == StencilMode::Write)
		{
			desc.StencilEnable = TRUE;
			desc.StencilWriteMask = 0xFF;
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		}
		else if (stencilMode == StencilMode::Mask)
		{
			desc.DepthEnable = FALSE;
			desc.StencilEnable = TRUE;
			desc.StencilReadMask = 0xFF;
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		}

		GFX_THROW_INFO(GetDevice(gfx)->CreateDepthStencilState(&desc, &m_pState));
	}
	void DepthStencil::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->OMSetDepthStencilState(m_pState.Get(), 0);
	}
	std::string DepthStencil::GetUID() const noexcept
	{
		return GenerateUID(m_depthFunc, m_depth, m_stencilFunc, m_stencilMode);
	}
	std::shared_ptr<DepthStencil> DepthStencil::Resolve(Graphics& gfx, Func depthFunc, bool depthEnable, Func stencilFunc, StencilMode stencilMode)
	{
		return Codex::Resolve<DepthStencil>(gfx, depthFunc, depthEnable, stencilFunc, stencilMode);
	}
	std::string DepthStencil::GenerateUID(Func depthFunc, bool depthEnable, Func stencilFunc, StencilMode stencilMode)
	{
		return ConvertFuncToString(depthFunc) + "#" + ConvertFuncToString(stencilFunc) + "#" + ConvertModeToString(stencilMode);
	}
}