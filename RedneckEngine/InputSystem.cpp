#include "InputSystem.h"

// @ Allocating memory for declared in static class members

KeyState InputSystem::m_iKeyMap[256];
std::function<void()> InputSystem::m_Hotkeys[256];

// @ Class definition

void InputSystem::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ProcessMessage(msg, wParam, lParam);
}

bool InputSystem::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		return ProcessKeybdMessage(uMsg, wParam, lParam);
	}
	return false;
}

bool InputSystem::ProcessKeybdMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto key = wParam;
	auto state = KeyState::None;

	switch (uMsg)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		state = KeyState::Down;
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		state = KeyState::Up;
		break;
	default:
		return false;
	}

	if (state == KeyState::Up && m_iKeyMap[int(key)] == KeyState::Down)
	{
		m_iKeyMap[int(key)] = KeyState::Pressed;

		auto& hotkey_callback = m_Hotkeys[key];

		if (hotkey_callback)
			hotkey_callback();
	}
	else
		m_iKeyMap[int(key)] = state;

	return true;
}

KeyState InputSystem::GetKeyState(std::uint32_t vk)
{
	return m_iKeyMap[vk];
}
bool InputSystem::IsKeyDown(std::uint32_t vk)
{
	return m_iKeyMap[vk] == KeyState::Down;
}
bool InputSystem::WasKeyPressed(std::uint32_t vk)
{
	if (m_iKeyMap[vk] == KeyState::Pressed) {
		m_iKeyMap[vk] = KeyState::Up;
		return true;
	}
	return false;
}

void InputSystem::RegisterHotkey(uint32_t vk, std::function<void()> f)
{
	m_Hotkeys[vk] = f;
}

void InputSystem::RemoveHotkey(uint32_t vk)
{
	m_Hotkeys[vk] = nullptr;
}