#pragma once
#include "Audio.h"
#include "WindowClass.h"
#include "Graphics.h"
#include "Timer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "Mesh.h"
#include "SolidSphere.h"
#include "Cubemap.h"
#include "PointLight.h"
#include "RedneckScene.h"
#include "ShadowMap.h"

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
	std::shared_ptr<Cubemap> m_cubemap;
	float m_clearColor[4] = { 0.07f, 0.0f, 0.05f, 1.0f };
	std::shared_ptr<PointLight> m_light;
	RedneckScene m_BasicScene;
	RedneckScene m_PBRScene;
};