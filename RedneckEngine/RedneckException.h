#pragma once
#include "WindowErrorClass.h"
#include "LogCommon.h"
#include <sstream>

#define THROW_LAST_EXCEPT() { \
	auto _errString = WindowErrorClass::GetLastError();   \
	std::ostringstream oss; \
	oss << "File: " << __FILE__ << std::endl; \
	oss << "Line: " << __LINE__ << std::endl; \
	oss << _errString;	\
	_errString = oss.str(); \
	LOG_ERROR(_errString.c_str());  \
	throw std::exception(_errString.c_str()); \
	}