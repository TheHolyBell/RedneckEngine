#pragma once

class Graphics;

class IDrawable
{
public:
	virtual void Draw(Graphics& gfx) const noexcept(!IS_DEBUG) = 0;
	virtual ~IDrawable() = default;
};