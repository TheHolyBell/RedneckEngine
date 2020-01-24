#include <Windows.h>

#include "ConsoleClass.h"
#include "WindowClass.h"
#include "InputSystem.h"
#include "CommandLineProcessor.h"
#include "OpenFileDialog.h"
#include "ImGuiManager.h"
#include "Application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR CommandLine, int)
{
	CommandLineProcessor::Execute(CommandLine);
	Console::Initialize();

	ImGuiManager::Initialize();

	try
	{
		App app{ 1280, 800, "Hello" };

		app.Run();
	}
	catch (std::exception & exc)
	{
		Console::WriteLine(exc.what());
	}
	
	ImGuiManager::Shutdown();
	Console::Shutdown();

	return 0;
}