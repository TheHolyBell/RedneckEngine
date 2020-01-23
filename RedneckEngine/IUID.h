#pragma once
#include <string>

class IUID
{
public:
	virtual ~IUID() = default;
	virtual std::string GetUID() const noexcept = 0;
};