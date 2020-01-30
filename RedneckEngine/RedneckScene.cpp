#include "RedneckScene.h"

RedneckScene::RedneckScene()
{
}

void RedneckScene::AddEntity(std::shared_ptr<Entity> entity)
{
	m_EntityManager.AddEntity(entity);
	m_Menu.AddItem(entity);
}

void RedneckScene::AddItem(std::shared_ptr<IMenuViewable> item)
{
	m_Menu.AddItem(item);
}

void RedneckScene::RemoveEntity(const std::string& name)
{
	m_EntityManager.RemoveEntity(name);
}

std::shared_ptr<Entity> RedneckScene::GetEntity(const std::string& name)
{
	return m_EntityManager.GetEntity(name);
}

void RedneckScene::Update(float dt)
{
	m_EntityManager.Update(dt);
}

void RedneckScene::Render(Graphics& gfx)
{
	m_EntityManager.Render(gfx);
	m_Menu.Render(gfx, *this);
}

void RedneckScene::Clear()
{
	m_EntityManager.Clear();
	m_Menu.Clear();
}
