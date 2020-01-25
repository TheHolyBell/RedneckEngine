#include "RedneckUtility.h"
#include <sstream>
#include <iomanip>
#include <algorithm>


std::vector<std::string> StringHelper::TokenizeQuoted(const std::string& input)
{
	std::istringstream stream;
	stream.str(input);
	std::vector<std::string> tokens;
	std::string token;

	while (stream >> std::quoted(token))
	{
		tokens.push_back(std::move(token));
	}
	return tokens;
}

std::wstring StringHelper::ToWide(const std::string& narrow)
{
	wchar_t wide[512];
	mbstowcs_s(nullptr, wide, narrow.c_str(), _TRUNCATE);
	return wide;
}

std::string StringHelper::ToNarrow(const std::wstring& wide)
{
	char narrow[512];
	wcstombs_s(nullptr, narrow, wide.c_str(), _TRUNCATE);
	return narrow;
}

std::wstring StringHelper::GetDirectoryFromPath(const std::wstring& filepath)
{
	size_t off1 = filepath.find_last_of('\\');
	size_t off2 = filepath.find_last_of('/');
	if (off1 == std::wstring::npos && off2 == std::wstring::npos) //If no slash or backslash in path?
	{
		return L"";
	}
	if (off1 == std::wstring::npos)
	{
		return filepath.substr(0, off2);
	}
	if (off2 == std::wstring::npos)
	{
		return filepath.substr(0, off1);
	}
	//If both exists, need to use the greater offset
	return filepath.substr(0, std::max(off1, off2));
}

std::string StringHelper::GetDirectoryFromPath(const std::string& filepath)
{
	size_t off1 = filepath.find_last_of('\\');
	size_t off2 = filepath.find_last_of('/');
	if (off1 == std::string::npos && off2 == std::string::npos) //If no slash or backslash in path?
	{
		return "";
	}
	if (off1 == std::string::npos)
	{
		return filepath.substr(0, off2);
	}
	if (off2 == std::string::npos)
	{
		return filepath.substr(0, off1);
	}
	//If both exists, need to use the greater offset
	return filepath.substr(0, std::max(off1, off2));
}

std::wstring StringHelper::GetFileNameOnlyFromPath(const std::wstring& filepath)
{
	size_t off1 = filepath.find_last_of('\\');
	size_t off2 = filepath.find_last_of('/');
	if (off1 == std::wstring::npos && off2 == std::wstring::npos) //If no slash or backslash in path?
	{
		return filepath.substr(0, filepath.find_last_of('.'));
	}
	size_t offset = 0;

	if (off1 != std::wstring::npos && off2 != std::wstring::npos)
	{
		offset = std::max(off1, off2);
	}
	else
	{
		if (off1 == std::wstring::npos)
			offset = off2;
		if (off2 == std::wstring::npos)
			offset = off1;
	}
	size_t period = filepath.find_last_of('.');
	if (period == std::wstring::npos) //if no period?
	{
		return filepath.substr(offset + 1, filepath.length() - offset);
	}
	else
	{
		return filepath.substr(offset + 1, period - offset - 1);
	}
}

std::wstring StringHelper::GetFileExtension(const std::wstring& filename)
{
	size_t off = filename.find_last_of('.');
	if (off == std::wstring::npos)
	{
		return {};
	}
	return std::wstring(filename.substr(off + 1));
}

std::string StringHelper::GetFileExtension(const std::string& filename)
{
	size_t off = filename.find_last_of('.');
	if (off == std::string::npos)
	{
		return {};
	}
	return std::string(filename.substr(off + 1));
}
