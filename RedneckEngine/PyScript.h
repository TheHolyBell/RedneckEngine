#pragma once
#include <string>
#include <cstdio>

class PyScript
{
public:
	PyScript(const std::string& filename);
	void Execute();
private:
	FILE* m_pFile = nullptr;
	std::string m_filename;
};