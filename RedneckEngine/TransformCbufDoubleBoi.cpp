#include "TransformCbufDoubleBoi.h"

namespace Bind
{
	std::unique_ptr<PixelConstantBuffer<TransformCbuf::Transforms>> TransformCbufDoubleBoi::m_pPcbuf;

	TransformCbufDoubleBoi::TransformCbufDoubleBoi(Graphics& gfx, const Entity& parent, UINT slotV, UINT slotP)
		: TransformCbuf(gfx, parent, slotV)
	{
		if (m_pPcbuf == nullptr)
			m_pPcbuf = std::make_unique<PixelConstantBuffer<Transforms>>(gfx, slotP);
	}
	void TransformCbufDoubleBoi::Bind(Graphics& gfx) noexcept
	{
		const auto tf = GetTransforms(gfx);
		TransformCbuf::UpdateBindImpl(gfx, tf);
		UpdateBindImpl(gfx, tf);
	}
	void TransformCbufDoubleBoi::UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcept
	{
		m_pPcbuf->Update(gfx, tf);
		m_pPcbuf->Bind(gfx);
	}
}