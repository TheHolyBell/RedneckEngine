#include "IBindable.h"

namespace Bind
{
	ID3D11DeviceContext* IBindable::GetContext(Graphics& gfx) noexcept
	{
		return gfx.m_pDeviceContext.Get();
	}
	ID3D11Device* IBindable::GetDevice(Graphics& gfx) noexcept
	{
		return gfx.m_pDevice.Get();
	}
	DXGIInfoManager& IBindable::GetInfoManager(Graphics& gfx)
	{
#ifndef NDEBUG
		return gfx.infoManager;
#else
		throw std::logic_error("YouFuckedUp! (tried to access to gfx.infoManager in Release config)");
#endif
	}
}