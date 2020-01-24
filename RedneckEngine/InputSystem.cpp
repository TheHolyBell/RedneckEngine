#include "InputSystem.h"
#include "ImGui\imgui.h"
#include "Event.h"
#include "WindowEvents.h"
#include "KeyboardEvents.h"
#include "MouseEvents.h"
#include "WindowClass.h"

// @ Allocating memory for declared in static class members

KeyState InputSystem::m_iKeyMap[256];
std::function<void()> InputSystem::m_Hotkeys[256];
int InputSystem::m_lastPressed = 0;

POINT InputSystem::m_MousePosition = {};
bool InputSystem::m_bMouseInWindow = false;
bool InputSystem::m_bMouseButtonIsDown[(int)MouseButton::TOTAL_MOUSE_BUTTONS];

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
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:                  
	case WM_LBUTTONUP:                    
	case WM_LBUTTONDBLCLK:                
	case WM_RBUTTONDOWN:                  
	case WM_RBUTTONUP:                    
	case WM_RBUTTONDBLCLK:                
	case WM_MBUTTONDOWN:                  
	case WM_MBUTTONUP:                    
	case WM_MBUTTONDBLCLK:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:       
	case WM_XBUTTONUP:                   
	case WM_XBUTTONDBLCLK:               
		return ProcessMouseMessage(uMsg, wParam, lParam);
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
		m_lastPressed = wParam;
		state = KeyState::Down;
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		m_lastPressed = 0;
		state = KeyState::Up;
		break;
	}

	EventDispatcher::DispatchGlobalEvent<KeyPressedEvent>((size_t)key, false);

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

bool InputSystem::ProcessMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// @ THIS IS A REAL MESSY. This function doesn't create more window, since in window class we're using singleton. It is used for other purposes
	static auto _wnd_ptr = Window::CreateInstance(69, 69, "Title");
	switch (uMsg)
	{
	case WM_MOUSEMOVE:
	{
		POINTS pos = MAKEPOINTS(lParam);
		if (pos.x >= 0 && pos.x < (SHORT)_wnd_ptr->GetWidth() && pos.y >= 0 && pos.y < (SHORT)_wnd_ptr->GetHeight())
		{
			OnMouseMoved({ pos.x, pos.y });
			if (!IsMouseInWindow())
			{
				SetCapture(_wnd_ptr->GetHandle());
				OnMouseEnter();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON | MK_XBUTTON1 | MK_XBUTTON2))
			{
				OnMouseMoved({ pos.x, pos.y });
			}
			else
			{
				ReleaseCapture();
				OnMouseLeave();
			}
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		POINTS pos = MAKEPOINTS(lParam);
		float delta = (float)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
		OnMouseWheelScrolled({ pos.y, pos.x }, delta);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		POINTS pos = MAKEPOINTS(lParam);
		OnMouseButtonDown({ pos.x, pos.y }, MouseButton::Left);
		break;
	}
	case WM_LBUTTONUP:
	{
		POINTS pos = MAKEPOINTS(lParam);
		OnMouseButtonUp({ pos.x, pos.y }, MouseButton::Left);
		break;
	}
	case WM_LBUTTONDBLCLK:
	{
		POINTS pos = MAKEPOINTS(lParam);
		OnMouseButtonDblClick({ pos.x, pos.y }, MouseButton::Left);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		POINTS pos = MAKEPOINTS(lParam);
		OnMouseButtonDown({ pos.x, pos.y }, MouseButton::Right);
		break;
	}
	case WM_RBUTTONUP:
	{
		POINTS pos = MAKEPOINTS(lParam);
		OnMouseButtonUp({ pos.x, pos.y }, MouseButton::Right);
		break;
	}
	case WM_RBUTTONDBLCLK:
	{
		POINTS pos = MAKEPOINTS(lParam);
		OnMouseButtonDblClick({ pos.x, pos.y }, MouseButton::Right);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		POINTS pos = MAKEPOINTS(lParam);
		OnMouseButtonDown({ pos.x, pos.y }, MouseButton::Middle);
		break;
	}
	case WM_MBUTTONUP:
	{
		POINTS pos = MAKEPOINTS(lParam);
		OnMouseButtonUp({ pos.x, pos.y }, MouseButton::Middle);
		break;
	}
	case WM_MBUTTONDBLCLK:
	{
		POINTS pos = MAKEPOINTS(lParam);
		OnMouseButtonDblClick({ pos.x, pos.y }, MouseButton::Middle);
		break;
	}
	case WM_XBUTTONDOWN:
	{
		POINTS pos = MAKEPOINTS(lParam);
		if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
		{
			OnMouseButtonDown({ pos.x, pos.y }, MouseButton::X1);
		}
		else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2)
		{
			OnMouseButtonDown({ pos.x, pos.y }, MouseButton::X2);
		}
		break;
	}
	case WM_XBUTTONUP:
	{
		POINTS pos = MAKEPOINTS(lParam);
		if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
		{
			OnMouseButtonUp({ pos.x, pos.y }, MouseButton::X1);
		}
		else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2)
		{
			OnMouseButtonUp({ pos.x, pos.y }, MouseButton::X2);
		}
		break;
	}
	case WM_XBUTTONDBLCLK:
	{
		POINTS pos = MAKEPOINTS(lParam);
		if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
		{
			OnMouseButtonDblClick({ pos.x, pos.y }, MouseButton::X1);
		}
		else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2)
		{
			OnMouseButtonDblClick({ pos.x, pos.y }, MouseButton::X2);
		}
		break;
	}
	}

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

int InputSystem::GetLastPressedKey()
{
	return m_lastPressed;
}


void InputSystem::OnMouseMoved(const POINT& pos)
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
	{
		return;
	}

	EventDispatcher::DispatchGlobalEvent<MouseMovedEvent>(pos, m_MousePosition);
	
	m_MousePosition = pos;
}

void InputSystem::OnMouseWheelScrolled(const POINT& pos, const float delta)
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
	{
		return;
	}

	m_MousePosition = pos;

	EventDispatcher::DispatchGlobalEvent<MouseScrolledEvent>(pos, delta);
}

void InputSystem::OnMouseButtonDown(const POINT& pos, const MouseButton mb)
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
	{
		return;
	}

	m_MousePosition = pos;
	m_bMouseButtonIsDown[(int)mb] = true;

	EventDispatcher::DispatchGlobalEvent<MouseButtonPressedEvent>(pos, mb);
}

void InputSystem::OnMouseButtonUp(const POINT& pos, const MouseButton mb)
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
	{
		return;
	}

	m_MousePosition = pos;
	m_bMouseButtonIsDown[(int)mb] = false;

	EventDispatcher::DispatchGlobalEvent<MouseButtonReleasedEvent>(pos, mb);
}

void InputSystem::OnMouseButtonDblClick(const POINT& pos, const MouseButton mb)
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
	{
		return;
	}

	m_MousePosition = pos;
	m_bMouseButtonIsDown[(int)mb] = true;

	EventDispatcher::DispatchGlobalEvent<MouseButtonDoubleClickEvent>(pos, mb);
}

void InputSystem::OnMouseEnter()
{
	m_bMouseInWindow = true;

	EventDispatcher::DispatchGlobalEvent<MouseEnterEvent>();
}

void InputSystem::OnMouseLeave()
{
	m_bMouseInWindow = false;

	EventDispatcher::DispatchGlobalEvent<MouseLeaveEvent>();
}

POINT InputSystem::GetMousePosition()
{
	return m_MousePosition;
}

bool InputSystem::IsMouseButtonDown(const MouseButton mb)
{
	return m_bMouseButtonIsDown[(int)mb];
}

bool InputSystem::IsLeftDown()
{
	return IsMouseButtonDown(MouseButton::Left);
}

bool InputSystem::IsRightDown()
{
	return IsMouseButtonDown(MouseButton::Right);
}

bool InputSystem::IsMiddleDown()
{
	return IsMouseButtonDown(MouseButton::Middle);
}

bool InputSystem::IsX1Down()
{
	return IsMouseButtonDown(MouseButton::X1);
}

bool InputSystem::IsX2Down()
{
	return IsMouseButtonDown(MouseButton::X2);
}

bool InputSystem::IsMouseInWindow()
{
	return m_bMouseInWindow;
}