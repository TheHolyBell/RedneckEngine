#include "PyScript.h"
#include <Python.h>
#include <exception>
#include "ConsoleClass.h"

PyScript::PyScript(const std::string& filename)
	: m_filename(filename)
{

}

void PyScript::Execute()
{
	if (fopen_s(&m_pFile, m_filename.c_str(), "r") != 0)
		throw std::exception("You fucked up");
	PyRun_SimpleFile(m_pFile, m_filename.c_str());
}
