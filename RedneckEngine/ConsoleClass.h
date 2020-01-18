#pragma once
#include <Windows.h>
#include <string>

class Console
{
public:
	static void Initialize();

	static bool WriteLine(const char* fmt, ...);
	static bool Write(const char* fmt, ...);
	
	static bool WriteLine(const std::string& fmt);
	static bool Write(const std::string&  fmt);

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