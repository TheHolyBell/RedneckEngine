#pragma once
#include "ConstantBuffers.h"
#include "Entity.h"
#include <DirectXMath.h>

namespace Bind
{
	class TransformCbuf : public IBindable
	{
	protected:
		struct Transforms
		{
			DirectX::XMMATRIX model;
			DirectX::XMMATRIX modelViewProj;
		};
	public:
		TransformCbuf(Graphics& gfx, const Entity& parent, UINT slot = 0);
		virtual void Bind(Graphics& gfx) noexcept override;
	protected:
		void UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcept;
		Transforms GetTransforms(Graphics& gfx) noexcept;
	private:
		static std::unique_ptr<VertexConstantBuffer<Transforms>> m_pVcbuf;
		const Entity& m_parent;
	};
}