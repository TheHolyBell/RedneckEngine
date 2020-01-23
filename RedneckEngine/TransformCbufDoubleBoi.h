#pragma once
#include "TransformCbuf.h"

namespace Bind
{
	class TransformCbufDoubleBoi : public TransformCbuf
	{
	public:
		TransformCbufDoubleBoi(Graphics& gfx, const Entity& entity, UINT slotV = 0u, UINT slotP = 0u);
		virtual void Bind(Graphics& gfx) noexcept override;
	protected:
		void UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcept;
	private:
		static std::unique_ptr<PixelConstantBuffer<Transforms>> m_pPcbuf;
	};
}