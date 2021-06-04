#include "Graphics.h"

#pragma comment(lib, "d3d11.lib")

/****************************EXCEPTIONS****************************/
/****************************EXCEPTIONS****************************/

Graphics::InfoException::InfoException(int line, const wchar_t* file, std::vector<std::wstring> info) noexcept
	: Exception{ line, file }
{
	for (const auto& s : info)
	{
		this->info += s;
		this->info.push_back('\n');
		this->info.push_back('\n');
	}
	if (!this->info.empty())
	{
		info.pop_back();
	}
}

const wchar_t* Graphics::InfoException::What() const noexcept
{
	std::wstringstream oss;
	oss << GetType() << std::endl << std::endl
		<< L"[Error Info]: " << GetErrorInfo() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const wchar_t* Graphics::HRException::What() const noexcept
{
	std::wstringstream oss;
	oss << GetType() << std::endl << std::endl
		<< L"[Error code]: " << std::hex << std::showbase << GetErrorCode() << std::endl
		<< L"[Description]: " << GetErrorString() << std::endl
		<< L"[Error Info]: " << GetErrorInfo() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

std::wstring Graphics::HRException::TranslateErrorCode(HRESULT hr) noexcept
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

/****************************END*EXCEPTIONS****************************/
/****************************END*EXCEPTIONS****************************/



namespace wrl = Microsoft::WRL;

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

	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// for exception throwing, do NOT rename
	HRESULT hr;

	UINT swapChainCreateFlags = 0u;
#ifndef NDEBUG
	swapChainCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapChainCreateFlags,
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
	wrl::ComPtr<ID3D11Resource> pBackBuffer;

	GFX_THROW_INFO(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));

	GFX_THROW_INFO(pDevice->CreateRenderTargetView
	(
		pBackBuffer.Get(),
		nullptr,
		&pTarget
	));

	// create depth stencil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	wrl::ComPtr<ID3D11DepthStencilState> pDepthStencilState;
	GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dsDesc, &pDepthStencilState));

	// bind depth stencil state
	pContext->OMSetDepthStencilState(pDepthStencilState.Get(), 1u);

	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = 800u;
	depthDesc.Height = 600u;
	depthDesc.MipLevels = 1u;
	depthDesc.ArraySize = 1u;
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.SampleDesc.Count = 1u;
	depthDesc.SampleDesc.Quality = 0u;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GFX_THROW_INFO(pDevice->CreateTexture2D(&depthDesc, nullptr, &pDepthStencil));

	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc = {};
	DSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Texture2D.MipSlice = 0u;
	GFX_THROW_INFO(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &DSVDesc, &pDepthStencilView));

	// bind depth stencil view to OM
	GFX_THROW_INFO_ONLY(pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDepthStencilView.Get()));
}

void Graphics::EndFrame()
{
	// for exception throwing, do NOT rename
	HRESULT hr;

#ifndef NDEBUG
	INFOMAN_SET;
#endif

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

void Graphics::ClearBuffer(float r, float g, float b) noexcept
{
	const float color[] = { r, g, b, 1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
	pContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}