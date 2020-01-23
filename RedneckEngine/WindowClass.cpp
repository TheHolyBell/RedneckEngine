#include "WindowClass.h"
#include "LogCommon.h"
#include "ExceptionMacro.h"
#include "InputSystem.h"
#include "ImGui\imgui_impl_win32.h"

Window::WindowClass Window::WindowClass::wndClass;

const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

Window::WindowClass::WindowClass() noexcept
	: hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.hInstance = hInst;
	wc.lpszClassName = wndClassName;

	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, hInst);
}

Window::Window(int Width, int Height, const char* Title)
	: m_width(Width),
	m_height(Height)
{
	// Calculate window size based on desired client region size
	RECT rc = {};
	rc.left = 0;
	rc.top = 0;
	rc.right = m_width;
	rc.bottom = m_height;
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	m_width = rc.right;
	m_height = rc.bottom;

	m_hWnd = CreateWindowA(WindowClass::GetName(),
		Title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr,
		nullptr, WindowClass::GetInstance(), this);

	if (m_hWnd == nullptr)
	{
		THROW_LAST_EXCEPT();
	}

	ShowWindow(m_hWnd, SW_SHOW);

	// Init ImGui Win32 Impl
	ImGui_ImplWin32_Init(m_hWnd);

	// Register mouse raw input device
	RAWINPUTDEVICE rid = {};
	rid.usUsagePage = 0x01;	// mouse page
	rid.usUsage = 0x02;	// mouse usage
	rid.hwndTarget = nullptr;
	rid.dwFlags = 0;
	if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
		THROW_LAST_EXCEPT();
}

Window::~Window()
{
	ImGui_ImplWin32_Shutdown();
	DestroyWindow(m_hWnd);
}

Window* Window::CreateInstance(int Width, int Height, const char* Title)
{
	static Window _instance(Width, Height, Title);
	return &_instance;
}

HWND Window::GetHandle() noexcept
{
	return m_hWnd;
}

int Window::GetWidth() const noexcept
{
	return m_width;
}

int Window::GetHeight() const noexcept
{
	return m_height;
}

void Window::SetTitle(const std::string& title)
{
	SetWindowTextA(m_hWnd, title.c_str());
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Retrieve pointer to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// Forward message to window class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

LRESULT Window::HandleMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return 1;

	InputSystem::WndProc(hWnd, uMsg, wParam, lParam);
	
	const auto& imio = ImGui::GetIO();


	switch (uMsg)
	{
	case WM_SIZE:
		m_width = LOWORD(lParam);
		m_height = HIWORD(lParam);
		if (m_width != 0 && m_height != 0)
			OnResizeHandler(m_width, m_height);
		break;
	case WM_ACTIVATE:
		// confine/free cursor on window to foreground/background if cursor disabled
		if (!m_cursorEnabled)
		{
			if (wParam & WA_ACTIVE)
			{
				ConfineCursor();
				HideCursor();
			}
			else
			{
				FreeCursor();
				ShowCursor();
			}
		}
		break;

	case WM_KILLFOCUS:
		//kbd.ClearState();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	/*case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		InputSystem::WndProc(hWnd, uMsg, wParam, lParam);
		break;*/
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Window::EnableCursor() noexcept
{
	m_cursorEnabled = true;
	ShowCursor();
	EnableImGuiMouse();
	FreeCursor();
}

void Window::DisableCursor() noexcept
{
	m_cursorEnabled = false;
	HideCursor();
	DisableImGuiMouse();
	ConfineCursor();
}


void Window::ConfineCursor() noexcept
{
	RECT rect;
	GetClientRect(m_hWnd, &rect);
	MapWindowPoints(m_hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
	ClipCursor(&rect);
}

void Window::FreeCursor() noexcept
{
	ClipCursor(nullptr);
}

bool Window::CursorEnabled() const noexcept
{
	return m_cursorEnabled;
}

void Window::HideCursor() noexcept
{
	while (::ShowCursor(FALSE) >= 0);
}

void Window::ShowCursor() noexcept
{
	while (::ShowCursor(TRUE) < 0);
}

void Window::EnableImGuiMouse() noexcept
{
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void Window::DisableImGuiMouse() noexcept
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}