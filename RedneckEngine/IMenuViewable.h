#pragma once
#include <string>
#include "IUID.h"

class Graphics;

class IMenuViewable : public IUID
{
public:
	virtual ~IMenuViewable() = default;
	virtual bool IsMenuDrawable() const noexcept = 0;
	virtual void DrawMenu(Graphics& gfx) noexcept = 0;
	virtual void ItemSelected() noexcept = 0;
};