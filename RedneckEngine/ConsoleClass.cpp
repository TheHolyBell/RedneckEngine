#include "ConsoleClass.h"

#include <cstdio>
#include <string>


HANDLE Console::_out = nullptr;
HANDLE Console::_old_out = nullptr;
HANDLE Console::_err = nullptr;
HANDLE Console::_old_err = nullptr;
HANDLE Console::_in = nullptr;
HANDLE Console::_old_in = nullptr;

void Console::Initialize()
{
	AttachConsole();
}

bool Console::WriteLine(const char* fmt, ...)
{
	if (_out == nullptr)
		return false;

	char buf[1024];
	va_list va;

	va_start(va, fmt);
	_vsnprintf_s(buf, 1024, fmt, va);
	va_end(va);

	std::string _buffie(buf);
	_buffie += "\n";

	return !!WriteConsoleA(_out, _buffie.c_str(), static_cast<DWORD>(_buffie.length()), nullptr, nullptr);
}

bool Console::Write(const char* fmt, ...)
{
	if (_out == nullptr)
		return false;

	char buf[1024];
	va_list va;

	va_start(va, fmt);
	_vsnprintf_s(buf, 1024, fmt, va);
	va_end(va);

	return !!WriteConsoleA(_out, buf, static_cast<DWORD>(strlen(buf)), nullptr, nullptr);
}

bool Console::WriteLine(const std::string& fmt)
{
	return WriteLine(fmt.c_str());
}

bool Console::Write(const std::string& fmt)
{
	return Write(fmt.c_str());
}


unsigned char Console::ReadKey()
{
	if (_in == nullptr)
		return false;

	auto key = char{ 0 };
	auto keysread = DWORD{ 0 };

	ReadConsoleA(_in, &key, 1, &keysread, nullptr);
	return key;
}


void Console::Shutdown()
{
	DetachConsole();
}

void Console::AttachConsole()
{
	_old_out = GetStdHandle(STD_OUTPUT_HANDLE);
	_old_err = GetStdHandle(STD_ERROR_HANDLE);
	_old_in = GetStdHandle(STD_INPUT_HANDLE);

	::AllocConsole() && ::AttachConsole(GetCurrentProcessId());

	_out = GetStdHandle(STD_OUTPUT_HANDLE);
	_err = GetStdHandle(STD_ERROR_HANDLE);
	_in = GetStdHandle(STD_INPUT_HANDLE);

	SetConsoleMode(_out,
		ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);

	SetConsoleMode(_in,
		ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS |
		ENABLE_PROCESSED_INPUT | ENABLE_QUICK_EDIT_MODE);
}

void Console::DetachConsole()
{
	if (_out && _err && _in)
	{
		FreeConsole();

		if (_old_out)
			SetStdHandle(STD_OUTPUT_HANDLE, _old_out);
		if (_old_err)
			SetStdHandle(STD_ERROR_HANDLE, _old_err);
		if (_old_in)
			SetStdHandle(STD_INPUT_HANDLE, _old_in);
	}
}
