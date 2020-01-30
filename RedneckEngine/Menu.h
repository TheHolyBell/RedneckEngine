#pragma once

#include "IMenuViewable.h"
#include <memory>
#include <unordered_map>
#include <string>

class RedneckScene;

class Menu
{
public:
	void Render(Graphics& gfx, RedneckScene& scene);
	void AddItem(std::shared_ptr<IMenuViewable> item);
	void RemoveItem(const std::string& name);
	void Clear();
private:
	std::unordered_map<std::string, std::shared_ptr<IMenuViewable>> m_items;
};