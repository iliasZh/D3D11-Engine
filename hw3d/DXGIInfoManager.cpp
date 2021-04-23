#include "DXGIInfoManager.h"
#include "Window.h"
#include "Graphics.h"
#include <memory>
#include "Utilities.h"

#pragma comment(lib, "dxguid.lib")

DXGIInfoManager::DXGIInfoManager()
{
	// define function signature of DXGIGetDebugInterface
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);
	
	// load the dll that contains the function DXGIGetDebugInterface
	const auto hModDXGIDebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);

	if (hModDXGIDebug == NULL)
	{
		throw CHWND_LAST_EXCEPT();
	}

	// find address of DXGIGetDebugInterface in the dll
	const auto DXGIGetDebugInterface_ = reinterpret_cast<DXGIGetDebugInterface>
		(
			reinterpret_cast<void*>(GetProcAddress(hModDXGIDebug, "DXGIGetDebugInterface"))
		);
	if (DXGIGetDebugInterface_ == NULL)
	{
		throw CHWND_LAST_EXCEPT();
	}

	HRESULT hr;
	GFX_THROW_NO_INFO(DXGIGetDebugInterface_(__uuidof(IDXGIInfoQueue), &pDXGIInfoQueue));
}

void DXGIInfoManager::Set() noexcept
{
	// set the index (next) so that the next call to GetMessages()
	// will only get errors generated after this call
	next = pDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::wstring> DXGIInfoManager::GetMessages() const
{
	std::vector<std::wstring> messages;
	const auto end = pDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = next; i < end; ++i)
	{
		HRESULT hr;
		SIZE_T messageLength;
		// get the size of ith message in bytes
		GFX_THROW_NO_INFO(pDXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));
		// allocate memory for message
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		// get the message and push its description to the vector
		GFX_THROW_NO_INFO(pDXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
		messages.emplace_back(to_ws(pMessage->pDescription));
	}
	return messages;
}
