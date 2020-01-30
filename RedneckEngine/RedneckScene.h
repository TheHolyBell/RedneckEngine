#pragma once
#include "Entity.h"
#include "EntityManager.h"
#include "Menu.h"

class RedneckScene
{
	//friend class Menu;
public:
	RedneckScene();

	void AddEntity(std::shared_ptr<Entity> entity);
	void AddItem(std::shared_ptr<IMenuViewable> item);

	void RemoveEntity(const std::string& name);
	std::shared_ptr<Entity> GetEntity(const std::string& name);

	void Update(float dt);
	void Render(Graphics& gfx);
	void Clear();
private:
	Menu m_Menu;
	EntityManager m_EntityManager;
};