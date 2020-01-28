#include "PyScriptManager.h"
#include "ConsoleClass.h"
#include <Python.h>
std::unordered_map<std::string, std::shared_ptr<PyScript>> PyScriptManager::m_Scripts;

void PyScriptManager::Initialize()
{
	Py_Initialize();
}

void PyScriptManager::AddScript(const std::string& Name, const std::string& ScriptName)
{
	m_Scripts[Name] = std::make_shared<PyScript>(ScriptName);
}

std::shared_ptr<PyScript> PyScriptManager::GetScript(const std::string& Name)
{
	return m_Scripts.at(Name);
}


void PyScriptManager::Execute(const std::string& Name)
{
	m_Scripts.at(Name)->Execute();
}
