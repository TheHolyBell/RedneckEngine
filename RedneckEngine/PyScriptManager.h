#pragma once
#include "PyScript.h"
#include <unordered_map>
#include <memory>

class PyScriptManager
{
public:
	static void Initialize();
	static void AddScript(const std::string& Name, const std::string& ScriptName);
	static std::shared_ptr<PyScript> GetScript(const std::string& Name);
	static void Execute(const std::string& Name);
private:
	static std::unordered_map<std::string, std::shared_ptr<PyScript>> m_Scripts;
};