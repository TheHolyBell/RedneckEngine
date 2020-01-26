#pragma once

#include "IMenuViewable.h"
#include <memory>
#include <unordered_map>
#include <string>

class Menu
{
public:
	static void Render(Graphics& gfx);
	static void AddItem(std::shared_ptr<IMenuViewable> item);
	static void RemoveItem(const std::string& name);
	static void Clear();
private:
	static std::unordered_map<std::string, std::shared_ptr<IMenuViewable>> m_items;
};