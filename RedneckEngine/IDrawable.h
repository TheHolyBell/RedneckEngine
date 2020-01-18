#pragma once

class IDrawable
{
public:
	virtual void Draw() const noexcept = 0;
	virtual ~IDrawable() = default;
};