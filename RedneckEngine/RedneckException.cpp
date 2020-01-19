#include "RedneckException.h"
#include <sstream>

RedneckException::RedneckException(int line, const char* file) noexcept
	: line(line), file(file)
{
}

const char* RedneckException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* RedneckException::GetType() const noexcept
{
	return "Redneck Exception";
}

int RedneckException::GetLine() const noexcept
{
	return line;
}

const std::string& RedneckException::GetFile() const noexcept
{
	return file;
}

std::string RedneckException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}