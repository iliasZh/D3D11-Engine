#pragma once

#include "ChiliWin.h"
#include "ChiliException.h"
#include <d3d11.h>
#include "DXGIInfoManager.h"
#include "GraphicsThrowMacros.h"
#include <wrl.h>

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
	~Graphics() = default;
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	void EndFrame();
	void ClearBuffer(float r, float g, float b) noexcept;
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
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
};

