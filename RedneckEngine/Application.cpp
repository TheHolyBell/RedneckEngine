#include "Application.h"
#include "EntityManager.h"

#pragma comment(lib, "assimp-vc140-mt.lib")

App::App(int width, int height, const char* title, const std::string& commandLine)
	:
	m_commandLine(commandLine)
{
	m_wnd = Window::CreateInstance(width, height, title);
	m_pGfx = std::make_unique<Graphics>(m_wnd);
	
	EntityManager::AddEntity(std::make_shared<SolidSphere>(*m_pGfx, 1));
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
	m_pGfx->BeginFrame(m_clearColor);

	EntityManager::Render(*m_pGfx);

	m_pGfx->EndFrame();
}
