#pragma once
#include <exception>
#include <string>

class RedneckException : public std::exception
{
public:
	RedneckException(int line, const char* file) noexcept;

	virtual const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;
private:
	int line = 0;
	std::string file = "NoName";
protected:
	mutable std::string whatBuffer;
};