#pragma once
#include <Windows.h>

class Console
{
public:
	static void Initialize();

	static bool WriteLine(const char* fmt, ...);
	static unsigned char ReadKey();

	static void Shutdown();
private:
	static void AttachConsole();
	static void DetachConsole();

private:
	static HANDLE _out, _old_out;
	static HANDLE _err, _old_err;
	static HANDLE _in, _old_in;
};