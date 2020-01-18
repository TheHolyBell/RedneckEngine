#include <Windows.h>

#include "ConsoleClass.h"
#include "WindowClass.h"
#include "InputSystem.h"
#include "CommandLineProcessor.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR CommandLine, int)
{
	CommandLineProcessor::Execute(CommandLine);

	InputSystem::RegisterHotkey(VK_SPACE, []() {Console::WriteLine("I've been pressed"); });
	Console::Initialize();

	Console::WriteLine("Dick");
	Console::WriteLine("Pussy juice");
	Console::ReadKey();

	Window* window = Window::CreateInstance(800, 600, "Test");


	window->OnResizeHandler += [](int width, int height) {Console::WriteLine("Width: %d, Height: %d", width, height); };
	window->OnResizeHandler += [](int, int) {Console::WriteLine("Second handler"); };

	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Console::Shutdown();

	return 0;
}