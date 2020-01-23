#include "Menu.h"
#include "EntityManager.h"
#include "Graphics.h"
#include "ImGui\imgui.h"
#include "ConsoleClass.h"

std::unordered_map<std::string, std::shared_ptr<IMenuViewable>> Menu::m_items;

void Menu::Render(Graphics& gfx)
{
	if (ImGui::Begin("Entity list"))
	{
		static char chosen[60] = "Chose an Entity";
		if (ImGui::BeginCombo("Entities", chosen))
		{
			for (const auto& elem : m_items)
			{
				if (ImGui::Selectable(elem.first.c_str()))
				{
					strcpy_s(chosen, elem.first.c_str());
					break;
				}
			}
			ImGui::EndCombo();
		}
		if (ImGui::Button("Open control Window"))
		{
			auto it = m_items.find(chosen);
			if (it != m_items.end())
				it->second->ItemSelected();
		}
	}
	ImGui::End();

	for (auto& i : m_items)
		if (i.second->IsMenuDrawable())
			i.second->DrawMenu(gfx);
}

void Menu::AddItem(std::shared_ptr<IMenuViewable> item)
{
	std::string name = item->GetName();
	auto it = m_items.find(name);
	if (it == m_items.end())
		m_items[name] = std::move(item);
	else
	{
		Console::SetConsoleColor(ConsoleColor::Red);
		Console::WriteLine("%s already exists", name.c_str());
		Console::SetConsoleColor(ConsoleColor::White);
	}
}

void Menu::RemoveItem(const std::string& name)
{
	auto it = m_items.find(name);
	if (it != m_items.end())
		m_items.erase(name);
	else
	{
		Console::SetConsoleColor(ConsoleColor::Red);
		Console::WriteLine("%s doesn't exists", name.c_str());
		Console::SetConsoleColor(ConsoleColor::White);
	}
	EntityManager::RemoveEntity(name);
}