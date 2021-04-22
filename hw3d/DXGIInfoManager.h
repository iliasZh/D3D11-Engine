#pragma once

#include "ChiliWin.h"
#include <vector>
#include <string>

class DXGIInfoManager
{
public:
	DXGIInfoManager();
	~DXGIInfoManager();
	DXGIInfoManager(const DXGIInfoManager&) = delete;
	DXGIInfoManager& operator=(const DXGIInfoManager&) = delete;
	void Set() noexcept;
	std::vector<std::wstring> GetMessages() const;
private:
	unsigned long long next = 0u;
	struct IDXGIInfoQueue* pDXGIInfoQueue = nullptr;
};