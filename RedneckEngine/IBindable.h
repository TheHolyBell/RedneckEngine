#pragma once
#include "CommonD3D.h"
#include "Graphics.h"
#include "IUID.h"

namespace Bind
{
	class IBindable : public IUID
	{
	public:
		virtual ~IBindable() = default;

		virtual void Bind(Graphics& gfx) noexcept = 0;
		virtual std::string GetUID() const noexcept
		{
			assert(false && "This class has no implementation of GetUID()");
			return "";
		}

	protected:
		static ID3D11DeviceContext* GetContext(Graphics& gfx) noexcept;
		static ID3D11Device* GetDevice(Graphics& gfx) noexcept;
		static DXGIInfoManager& GetInfoManager(Graphics& gfx);
	};
}