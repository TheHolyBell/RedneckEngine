#include "Menu.h"
#include "EntityManager.h"
#include "Graphics.h"
#include "ImGui\imgui.h"
#include "ConsoleClass.h"
#include "Mesh.h"
#include "OpenFileDialog.h"

std::unordered_map<std::string, std::shared_ptr<IMenuViewable>> Menu::m_items;

std::string GetPath()
{
	std::string _path;
	
	/*static OPENFILENAME ofn = {};       // common dialog box structure
	static char szFile[260] = {};       // buffer for file name

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn) == TRUE)
		return ofn.lpstrFile;*/

	//return "Models\\boxy.gltf";
	auto p = OpenFileDialog::ShowDialog();
	if (p.has_value())
		return *p;
	else
		return "Models\\nanosuit.obj";
}

void Menu::Render(Graphics& gfx)
{
	if (ImGui::Begin("Entity list"))
	{
		static char chosen[256] = "Chose an Entity";
		if (ImGui::BeginCombo("Entities", chosen))
		{
			for (const auto& elem : m_items)
			{
				const auto& name = elem.first.c_str();
				if (ImGui::Selectable(name))
				{
					strcpy_s(chosen, name);
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
		if (ImGui::Button("Remove object"))
		{
			RemoveItem(chosen);
		}
	}
	
	if (ImGui::Button("Load new object"))
	{
		//
		auto path = GetPath();
		EntityManager::AddEntity(std::make_shared<Model>(gfx, path));
	}

	ImGui::End();

	for (auto& i : m_items)
		if (i.second->IsMenuDrawable())
			i.second->DrawMenu(gfx);
}

void Menu::AddItem(std::shared_ptr<IMenuViewable> item)
{
	std::string name = item->GetUID();
	auto it = m_items.find(name);
	if (it == m_items.end())
		m_items[name] = item;
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