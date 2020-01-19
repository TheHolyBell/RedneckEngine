#pragma once

#include <Windows.h>
#include <vector>
#include <wrl\client.h>
#include <dxgidebug.h>
#include <string>

class DXGIInfoManager
{
public:
	DXGIInfoManager();

	DXGIInfoManager(const DXGIInfoManager& rhs) = delete;
	DXGIInfoManager& operator=(const DXGIInfoManager& rhs) = delete;

	~DXGIInfoManager() = default;

	void Set() noexcept;
	std::vector<std::string> GetMessages()const;
private:
	unsigned long long next = 0;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> m_pDxgiInfoQueue = nullptr;
};