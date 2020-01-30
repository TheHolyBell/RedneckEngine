#pragma once
#include "Entity.h"
#include "Graphics.h"
#include <unordered_map>
#include <memory>

class EntityManager
{
public:
	void AddEntity(std::shared_ptr<Entity> entity);
	void RemoveEntity(const std::string& name);
	std::shared_ptr<Entity> GetEntity(const std::string& name);

	void Update(float dt);
	void Render(Graphics& gfx);
	void Clear();
private:
	std::unordered_map<std::string, std::shared_ptr<Entity>> m_entities;
};