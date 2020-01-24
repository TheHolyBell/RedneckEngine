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
public: // mouse
	enum class MouseButton
	{
		Left,
		Right,
		Middle,
		X1,
		X2,
		TOTAL_MOUSE_BUTTONS
	};
private:
	static void OnMouseMoved(const POINT& pos);
	static void OnMouseWheelScrolled(const POINT& pos, const float delta);
	static void OnMouseButtonDown(const POINT& pos, const MouseButton mb);
	static void OnMouseButtonUp(const POINT& pos, const MouseButton mb);
	static void OnMouseButtonDblClick(const POINT& pos, const MouseButton mb);
	static void OnMouseEnter();
	static void OnMouseLeave();
public:
	static POINT GetMousePosition();
	static bool IsMouseButtonDown(const MouseButton mb);

	static bool IsLeftDown();
	static bool IsRightDown();
	static bool IsMiddleDown();
	static bool IsX1Down();
	static bool IsX2Down();

	static bool IsMouseInWindow();
private:
	static void WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static bool ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	static bool ProcessKeybdMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	static bool ProcessMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	static POINT m_MousePosition;
	static bool m_bMouseInWindow;
	static bool m_bMouseButtonIsDown[(int)MouseButton::TOTAL_MOUSE_BUTTONS];

	static KeyState m_iKeyMap[256];
	static int m_lastPressed;
	static std::function<void()> m_Hotkeys[256];
};