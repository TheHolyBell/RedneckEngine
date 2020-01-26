#include <Windows.h>

#include "ConsoleClass.h"
#include "WindowClass.h"
#include "InputSystem.h"
#include "CommandLineProcessor.h"
#include "OpenFileDialog.h"
#include "ImGuiManager.h"
#include "Application.h"
#include <fstream>

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
	catch (std::exception& exc)
	{
		std::ofstream fout("ErrorLog.txt");
		Console::WriteLine(exc.what());
		fout << exc.what();
		Console::ReadKey();
	}
	
	ImGuiManager::Shutdown();
	Console::Shutdown();

	return 0;
}