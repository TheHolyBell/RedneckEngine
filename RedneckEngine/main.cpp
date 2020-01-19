#include <Windows.h>

#include "ConsoleClass.h"
#include "WindowClass.h"
#include "InputSystem.h"
#include "CommandLineProcessor.h"
#include "OpenFileDialog.h"
#include "ImGuiManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR CommandLine, int)
{
	CommandLineProcessor::Execute(CommandLine);
	Console::Initialize();

	InputSystem::WasKeyPressed(VK_SPACE);
	InputSystem::RegisterHotkey(VK_SPACE, []() {OpenFileDialog::ShowDialogAsync([](const std::string& path) {Console::WriteLine(path); }); });

	Console::SetConsoleColor(ConsoleColor::White);
	Console::WriteLine("Dick");
	Console::SetConsoleColor(ConsoleColor::Blue);
	Console::WriteLine("Pussy juice");

	Console::SetConsoleColor(ConsoleColor::Red);
	//Console::Write(*OpenFileDialog::ShowDialog());
	Console::WriteLine("Final number is: %d", Console::Read());
	Console::ReadKey();

	ImGuiManager::Initialize();

	Window* window = Window::CreateInstance(800, 600, "Redneck Engine");

	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Console::Shutdown();

	ImGuiManager::Shutdown();

	return 0;
}