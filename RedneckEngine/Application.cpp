#include "Application.h"
#include "EntityManager.h"
#include "OpenFileDialog.h"
#include "TestCube.h"
#include "TestPlane.h"
#include "InputSystem.h"
#include "ConsoleClass.h"
#include "Menu.h"
#include "Cubemap.h"
#include "TestCylinder.h"
#include <random>
#include "Math.h"

enum class EntityType
{
	Box,
	Plane,
	Cylinder
};

std::shared_ptr<Entity> EntityFactory(Graphics& gfx, EntityType type)
{
	static std::mt19937 rng{ std::random_device{}() };
	static std::uniform_int_distribution<int> boxdist{ 10,30 };
	static std::uniform_int_distribution<int> planedist{ 50,300 };
	static std::uniform_real_distribution<float> adist{ 0.0f,Math::PI * 2.0f };
	static std::uniform_real_distribution<float> ddist{ 0.0f,Math::PI * 0.5f };
	static std::uniform_real_distribution<float> odist{ 0.0f,Math::PI * 0.08f };
	static std::uniform_real_distribution<float> xdist{ 10.0f,70.0f };
	static std::uniform_real_distribution<float> ydist{ 10.0f,30.0f };
	static std::uniform_real_distribution<float> zdist{ 10.0f,60.0f };
	static std::uniform_int_distribution<int> tdist{ 3,30 };

	

	switch (type)
	{
	case EntityType::Box:
	{
		auto pBox = std::make_shared<TestCube>(gfx, boxdist(rng));
		pBox->SetPos({ xdist(rng), ydist(rng), zdist(rng)});
		return pBox;
	}
	case EntityType::Cylinder:
	{
		auto pCylinder = std::make_shared<TestCylinder>(gfx, 2, 0.01, 7);
		pCylinder->SetPos({ xdist(rng), ydist(rng), zdist(rng) });
		return pCylinder;
	}
	case EntityType::Plane:
	{
		auto pPlane = std::make_shared<TestPlane>(gfx, planedist(rng));
		return pPlane;
	}
	}
}

App::App(int width, int height, const char* title, const std::string& commandLine)
	:
	m_commandLine(commandLine)
{
	m_wnd = Window::CreateInstance(width, height, title);
	m_pGfx = std::make_unique<Graphics>(m_wnd);
	
	//EntityManager::AddEntity(std::make_shared<Model>(*m_pGfx, OpenFileDialog::ShowDialog(), 0.5f));
	//EntityManager::AddEntity(std::make_shared<SolidSphere>(*m_pGfx, 1));
	
	m_cubemap = std::make_shared<Cubemap>(*m_pGfx, "Images\\grasscube1024.dds");

	EntityManager::AddEntity(EntityFactory(*m_pGfx, EntityType::Box));
	EntityManager::AddEntity(EntityFactory(*m_pGfx, EntityType::Box));
	EntityManager::AddEntity(EntityFactory(*m_pGfx, EntityType::Box));
	EntityManager::AddEntity(EntityFactory(*m_pGfx, EntityType::Plane));
	EntityManager::AddEntity(EntityFactory(*m_pGfx, EntityType::Cylinder));
	EntityManager::AddEntity(EntityFactory(*m_pGfx, EntityType::Cylinder));
	Menu::AddItem(std::make_shared<MusicClass>());
	Menu::AddItem(m_cubemap);
}

int App::Run()
{
	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			DoFrame();
	}
	return -1;
}

void App::DoFrame()
{
	auto dt = m_timer.Mark();
	m_pGfx->BeginFrame(m_clearColor);
 

	auto& camera = m_pGfx->m_camera;
	camera.Update(dt);

	auto camPos = camera.GetPos();
	m_cubemap->SetTransform(DirectX::XMMatrixTranslation(camPos.x, camPos.y, camPos.z));
	m_cubemap->Draw(*m_pGfx);
	
	EntityManager::Update(dt);
	EntityManager::Render(*m_pGfx);
	Menu::Render(*m_pGfx);

	m_pGfx->EndFrame();
}
