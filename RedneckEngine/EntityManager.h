#pragma once
#include "Entity.h"
#include "Graphics.h"
#include <unordered_map>
#include <memory>

class EntityManager
{
	friend class Menu;
public:
	static void AddEntity(std::shared_ptr<Entity> entity);
	static void RemoveEntity(const std::string& name);
	static std::shared_ptr<Entity> GetEntity(const std::string& name);

	static void Update(float dt);
	static void Render(Graphics& gfx);

private:
	static std::unordered_map<std::string, std::shared_ptr<Entity>> m_entities;
};