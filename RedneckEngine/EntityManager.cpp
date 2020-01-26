#include "EntityManager.h"
#include "ConsoleClass.h"
#include "Menu.h"
#include <typeinfo>

std::unordered_map<std::string, std::shared_ptr<Entity>> EntityManager::m_entities;

void EntityManager::AddEntity(std::shared_ptr<Entity> entity)
{
	std::string _name = entity->GetUID();

	auto it = m_entities.find(_name);
	if (it == m_entities.end())
	{
		m_entities[_name] = entity;
		Menu::AddItem(entity);

		Console::WriteLine("%s successfully loaded", _name.c_str());
	}
	else
	{
		Console::SetConsoleColor(ConsoleColor::Red);
		Console::WriteLine("%s already exists", _name.c_str());
		Console::SetConsoleColor(ConsoleColor::White);
	}
}

void EntityManager::RemoveEntity(const std::string& name)
{
	auto it = m_entities.find(name);
	if (it != m_entities.end())
		m_entities.erase(name);
	else
	{
		Console::SetConsoleColor(ConsoleColor::Red);
		Console::WriteLine("%s doesn't exists", name.c_str());
		Console::SetConsoleColor(ConsoleColor::White);
	}
}

std::shared_ptr<Entity> EntityManager::GetEntity(const std::string& name)
{
	auto it = m_entities.find(name);
	if (it != m_entities.end())
		return m_entities[name];
	else
	{
		Console::SetConsoleColor(ConsoleColor::Red);
		Console::WriteLine("%s doesn't exists", name.c_str());
		Console::SetConsoleColor(ConsoleColor::White);
		return nullptr;
	}
}

void EntityManager::Update(float dt)
{
	for (auto& e : m_entities)
		e.second->Update(dt);
}

void EntityManager::Render(Graphics& gfx)
{
	for (auto& e : m_entities)
		e.second->Draw(gfx);
}

void EntityManager::Clear()
{
	m_entities = std::unordered_map<std::string, std::shared_ptr<Entity>>();
}
