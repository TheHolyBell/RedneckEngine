#pragma once
#include <Windows.h>
#include <unordered_map>

class WindowErrorClass
{
public:
	static std::string GetDescription(DWORD uMsg, LPARAM lParam, WPARAM wParam);
	static std::string GetLastError();
	static std::string TranslateErrorCode(HRESULT hr) noexcept;
private:
	static std::unordered_map<DWORD, std::string> m_map;
};