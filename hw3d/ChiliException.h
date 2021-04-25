#pragma once

#include <string>
#include <sstream>

class ChiliException
{
public:
	ChiliException(int line, const wchar_t* file) noexcept
		: line{ line }
		, file{ file }
	{}
	virtual const wchar_t* What() const noexcept
	{
		std::wstringstream oss;
		oss << GetType() << std::endl << std::endl
			<< GetOriginString();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}
	virtual const wchar_t* GetType() const noexcept
	{
		return L"Chili Exception";
	}
	int GetLine() const noexcept
	{
		return line;
	}
	const std::wstring& GetFile() const noexcept
	{
		return file;
	}
	std::wstring GetOriginString() const noexcept
	{
		std::wstringstream oss;
		oss << "[File]: " << file << std::endl
			<< "[Line]: " << line;
		return oss.str();
	}
private:
	int line;
	std::wstring file;
protected:
	mutable std::wstring whatBuffer;
};