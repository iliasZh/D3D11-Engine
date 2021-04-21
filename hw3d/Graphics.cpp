#include "Graphics.h"

#pragma comment(lib, "d3d11.lib")

// window exception stuff
Graphics::Exception::Exception(int line, const wchar_t* file, HRESULT hr) noexcept
	: ChiliException{ line, file }
	, hr{ hr }
{}

const wchar_t* Graphics::Exception::What() const noexcept
{
	std::wstringstream oss;
	oss << GetType() << std::endl
		<< L"[Error code]: " << std::hex << std::showbase << GetErrorCode() << std::endl
		<< L"[Description]: " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const wchar_t* Graphics::Exception::GetType() const noexcept
{
	return L"Chili Graphics Exception";
}

std::wstring Graphics::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	wchar_t* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage
	(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&pMsgBuf), 0, nullptr
	);
	if (nMsgLen == 0)
	{
		return L"Unidentified error code";
	}
	std::wstring errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Graphics::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::wstring Graphics::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}



Graphics::Graphics(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	// you (d3d) figure it out
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	
	// whatever
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	
	// no AA
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	
	// pipeline will render its stuff to this buffer
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	
	// 1 frontbuffer, 1 backbuffer
	sd.BufferCount = 1;

	sd.OutputWindow = /*hWnd*/ (HWND)696969;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// for exception throwing, do NOT rename
	HRESULT hr;

	GFX_THROW_IF_FAILED(D3D11CreateDeviceAndSwapChain
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	));

	// gain access to texture subresource in swap chain (back buffer)
	ID3D11Resource* pBackBuffer = nullptr;
	GFX_THROW_IF_FAILED(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer)));

	GFX_THROW_IF_FAILED(pDevice->CreateRenderTargetView
	(
		pBackBuffer,
		nullptr,
		&pTarget
	));

	pBackBuffer->Release();
}

Graphics::~Graphics()
{
	if (pContext)
		pContext->Release();
	
	if (pDevice)
		pDevice->Release();

	if (pSwap)
		pSwap->Release();

	if (pTarget)
		pTarget->Release();
}

void Graphics::EndFrame()
{
	// for exception throwing, do NOT rename
	HRESULT hr;
	if (FAILED(hr = pSwap->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		}
		else
		{
			throw GFX_EXCEPT(hr);
		}
	}
}

const wchar_t* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return L"Graphics device removed";
}
