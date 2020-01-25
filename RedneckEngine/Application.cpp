#include "Application.h"
#include "EntityManager.h"
#include "OpenFileDialog.h"
#include "TestCube.h"
#include "TestPlane.h"
#include "InputSystem.h"
#include "ConsoleClass.h"
#include "Menu.h"
#include "Cubemap.h"

App::App(int width, int height, const char* title, const std::string& commandLine)
	:
	m_commandLine(commandLine)
{
	m_wnd = Window::CreateInstance(width, height, title);
	m_pGfx = std::make_unique<Graphics>(m_wnd);
	
	//EntityManager::AddEntity(std::make_shared<Model>(*m_pGfx, OpenFileDialog::ShowDialog(), 0.5f));
	//EntityManager::AddEntity(std::make_shared<SolidSphere>(*m_pGfx, 1));
	
	m_cubemap = std::make_shared<Cubemap>(*m_pGfx, "Images\\grasscube1024.dds");

	EntityManager::AddEntity(std::make_shared<TestCube>(*m_pGfx, 10.0f));
	EntityManager::AddEntity(std::make_shared<TestCube>(*m_pGfx, 15.0f));
	EntityManager::AddEntity(std::make_shared<TestCube>(*m_pGfx, 30.0f));
	EntityManager::AddEntity(std::make_shared<TestPlane>(*m_pGfx, 100));
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
