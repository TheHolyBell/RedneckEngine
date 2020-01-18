#pragma once

class IMenuViewable
{
public:
	virtual void ViewMenu() noexcept = 0;
	virtual ~IMenuViewable() = default;
};