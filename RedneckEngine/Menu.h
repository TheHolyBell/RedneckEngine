#pragma once

#include "IMenuViewable.h"
#include <memory>
#include <vector>
#include <string>

class Menu
{
public:
	static void Render(Graphics& gfx);
	static void AddItem(std::shared_ptr<IMenuViewable> item);
private:
	static std::vector<std::shared_ptr<IMenuViewable>> m_items;
};