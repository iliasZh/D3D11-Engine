#pragma once

#include "ChiliWin.h"
#include "ChiliException.h"
#include <d3d11.h>
#include "DXGIInfoManager.h"

class Graphics
{
public:
	class Exception : public ChiliException
	{
	public:
		Exception(int line, const wchar_t* file, HRESULT hr, std::vector<std::wstring> info = {}) noexcept;
		const wchar_t* What() const noexcept override;
		const wchar_t* GetType() const noexcept override;
		static std::wstring TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::wstring GetErrorString() const noexcept;
		std::wstring GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::wstring info;
	};
	class DeviceRemovedException : public Exception
	{
	public:
		using Exception::Exception;
		const wchar_t* GetType() const noexcept override;
	};
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	~Graphics();
	Graphics& operator=(const Graphics&) = delete;
	void EndFrame();
	void ClearBuffer(float r, float g, float b) noexcept
	{
		const float color[] = { r, g, b, 1.0f };
		pContext->ClearRenderTargetView(pTarget, color);
	}
#ifndef NDEBUG
	DXGIInfoManager& GetInfoManager() noexcept
	{
		return infoMan;
	}
#endif
private:
#ifndef NDEBUG
	DXGIInfoManager infoMan;
#endif
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;
};

#define GFX_EXCEPT_NO_INFO(hr) Graphics::Exception{__LINE__, WFILE, (hr)}
#define GFX_THROW_NO_INFO(hrcall) if (FAILED(hr = (hrcall))) throw GFX_EXCEPT_NO_INFO(hr) 

#define INFOMAN_MSGS Graphics::GetInfoManager().GetMessages()
#define INFOMAN_SET Graphics::GetInfoManager().Set()

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::Exception{ __LINE__, WFILE, (hr), INFOMAN_MSGS }
#define GFX_THROW_INFO(hrcall) INFOMAN_SET; if ((hr = (hrcall)) != S_OK) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException{ __LINE__, WFILE, (hr), INFOMAN_MSGS }
#else // NDEBUG
#define GFX_EXCEPT(hr) Graphics::Exception{ __LINE__, WFILE, (hr) }
#define GFX_THROW_INFO(hrcall) GFX_THROW_NO_INFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException{ __LINE__, WFILE, (hr) }
#endif // NDEBUG