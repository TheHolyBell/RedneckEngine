#pragma once
#include "Audio.h"
#include "WindowClass.h"
#include "Graphics.h"
#include "Timer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "Mesh.h"
#include "SolidSphere.h"

#include <memory>
#include <set>

class App
{
public:
	App(int width, int height, const char* title, const std::string& commandLine = "");

	// Master frame / message loop
	int Run();

	~App() = default;
private:
	void DoFrame();
private:
	ImGuiManager imgui;
	Window* m_wnd = nullptr;
	std::unique_ptr<Graphics> m_pGfx;
	Timer m_timer;
	float speed_factor = 1.0f;
	std::string m_commandLine;
	float m_clearColor[4] = { 0.07f, 0.0f, 0.05f, 1.0f };
};