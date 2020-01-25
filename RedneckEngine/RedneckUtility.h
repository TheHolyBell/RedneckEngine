#pragma once
#include <vector>
#include <string>


class StringHelper
{
public:
	static std::vector<std::string> TokenizeQuoted(const std::string& input);
	static std::wstring ToWide(const std::string& narrow);
	static std::string ToNarrow(const std::wstring& wide);
	static std::wstring GetDirectoryFromPath(const std::wstring& filepath);
	static std::string GetDirectoryFromPath(const std::string& filepath);
	static std::wstring GetFileNameOnlyFromPath(const std::wstring& filepath);
	static std::wstring GetFileExtension(const std::wstring& filename);
	static std::string GetFileExtension(const std::string& filename);
};