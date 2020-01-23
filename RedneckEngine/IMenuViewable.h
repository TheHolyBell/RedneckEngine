#pragma once
#include <string>

class Graphics;

class IMenuViewable
{
public:
	virtual std::string GetName() const noexcept = 0;
	virtual bool IsMenuDrawable() const noexcept = 0;
	virtual void DrawMenu(Graphics& gfx) noexcept = 0;
	virtual void ItemSelected() noexcept = 0;
	virtual ~IMenuViewable() = default;
};