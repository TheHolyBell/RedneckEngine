#pragma once
#include <Windows.h>
#include <cstdint>
#include <functional>

enum class KeyState
{
	None = 0,
	Down,
	Up,
	Pressed
};

DEFINE_ENUM_FLAG_OPERATORS(KeyState);

class InputSystem
{
	friend class Window;
public:
	static KeyState GetKeyState(uint32_t vk);
	static bool IsKeyDown(uint32_t vk);
	static bool WasKeyPressed(uint32_t vk);

	static void RegisterHotkey(uint32_t vk, std::function<void()> f);
	static void RemoveHotkey(uint32_t vk);

	static int GetLastPressedKey();
private:
	static void WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static bool ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	static bool ProcessKeybdMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	static KeyState m_iKeyMap[256];
	static int m_lastPressed;
	static std::function<void()> m_Hotkeys[256];
};