#pragma once
#include <string>
#include <functional>
#include <optional>

class OpenFileDialog
{
public:
	
	static void ShowDialogAsync(std::function<void(const std::string&)> _callback);
	static std::optional<std::string> ShowDialog();
};