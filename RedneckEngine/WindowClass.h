#pragma once
#include <Windows.h>
#include <string>
#include "EventHandler.h"

class Window
{
public:
	static Window* CreateInstance(int Width, int Height, const char* Title);

	HWND GetHandle() noexcept;
	int GetWidth() const noexcept;
	int GetHeight() const noexcept;

	void SetTitle(const std::string& title);

	void EnableCursor() noexcept;
	void DisableCursor() noexcept;
	bool CursorEnabled() const noexcept;
private:
	Window(int Width, int Height, const char* Title);
	~Window();

	Window(const Window& rhs) = delete;
	Window& operator=(const Window& rhs) = delete;

private:
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();

		WindowClass(const WindowClass& rhs) = delete;
		WindowClass& operator=(const WindowClass& rhs) = delete;

		constexpr static const char* wndClassName = "Direct3D Engine Window";
		static WindowClass wndClass;
		HINSTANCE hInst = nullptr;
	};

public:
	// Events for notice listeners (Pattern: Observer)
	EventHandler<void(int, int)> OnResizeHandler;

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT CALLBACK HandleMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void HideCursor() noexcept;
	void ShowCursor() noexcept;
	void EnableImGuiMouse() noexcept;
	void DisableImGuiMouse() noexcept;
	void ConfineCursor() noexcept;
	void FreeCursor() noexcept;

private:
	bool m_cursorEnabled = true;
	int m_width = 0;
	int m_height = 0;
	HWND m_hWnd = nullptr;
};