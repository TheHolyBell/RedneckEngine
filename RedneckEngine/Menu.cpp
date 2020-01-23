#include "Menu.h"
#include "EntityManager.h"
#include "Graphics.h"

std::vector<std::shared_ptr<IMenuViewable>> Menu::m_items;

void Menu::Render(Graphics& gfx)
{
	auto& items = m_items;

	for (auto& i : items)
		if (i->IsMenuDrawable())
			i->DrawMenu(gfx);
}

void Menu::AddItem(std::shared_ptr<IMenuViewable> item)
{
	m_items.push_back(item);
}
