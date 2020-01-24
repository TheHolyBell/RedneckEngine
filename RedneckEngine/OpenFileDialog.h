#pragma once
#include <string>
#include <functional>
#include <optional>

class OpenFileDialog
{
public:
	
	// @ Callback will be invoked if file has been chosen. Otherwise it won't
	static void ShowDialogAsync(std::function<void(const std::string&)> _callback, const char* filter = "All Files(*.*)\0*.*\0");
	
	// @ Simple synchronous version of previous method, this one returns path if it has been chosen, otherwise - empty std::optional
	static std::optional<std::string> ShowDialog(const char* filter = "All Files(*.*)\0*.*\0");
};