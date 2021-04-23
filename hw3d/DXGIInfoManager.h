#pragma once

#include "ChiliWin.h"
#include <vector>
#include <string>
#include <dxgidebug.h>
#include <wrl.h>

class DXGIInfoManager
{
public:
	DXGIInfoManager();
	~DXGIInfoManager() = default;
	DXGIInfoManager(const DXGIInfoManager&) = delete;
	DXGIInfoManager& operator=(const DXGIInfoManager&) = delete;
	void Set() noexcept;
	std::vector<std::wstring> GetMessages() const;
private:
	unsigned long long next = 0u;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDXGIInfoQueue;
};