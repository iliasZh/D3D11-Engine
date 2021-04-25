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
	/****************************EXCEPTIONS****************************/
	/****************************EXCEPTIONS****************************/
	class Exception : public ChiliException
	{
	public:
		using ChiliException::ChiliException;
		const wchar_t* GetType() const noexcept override { return L"Chili Graphics Exception"; }
	};

	class InfoException : public Exception
	{
	public:
		InfoException(int line, const wchar_t* file, std::vector<std::wstring> info) noexcept;
		const std::wstring& GetErrorInfo() const noexcept { return info; }
		const wchar_t* What() const noexcept override;
		const wchar_t* GetType() const noexcept override { return L"Chili Graphics Info Exception"; }
	protected:
		std::wstring info;
	};
	
	class HRException : public InfoException
	{
	public:
		HRException(int line, const wchar_t* file, HRESULT hr, std::vector<std::wstring> info = {}) noexcept
			: InfoException{ line, file, info }
			, hr{ hr }
		{}
		const wchar_t* What() const noexcept override;
		static std::wstring TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept { return hr; }
		std::wstring GetErrorString() const noexcept { return TranslateErrorCode(hr); }
		const wchar_t* GetType() const noexcept override { return L"Chili Graphics Error"; }
	private:
		HRESULT hr;
	};
	
	class DeviceRemovedException : public HRException
	{
	public:
		using HRException::HRException;
		const wchar_t* GetType() const noexcept override { return L"Chili Graphics Error: Device removed"; }
	};
	/****************************END*EXCEPTIONS****************************/
	/****************************END*EXCEPTIONS****************************/
public:
	Graphics(HWND hWnd);
	~Graphics() = default;
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	void EndFrame();
	void ClearBuffer(float r, float g, float b) noexcept;
	
	void DrawTestTriangle()
	{
		// debug, do not rename
		HRESULT hr;

		struct Vertex
		{
			float x;
			float y;
		};

		const Vertex vertices[] = 
		{
			{0.0f, 0.0f}, 
			{0.0f, 0.5f},
			{0.5f, 0.0f}
		};

		namespace wrl = Microsoft::WRL;
		wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
		
		D3D11_BUFFER_DESC bd = {};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = sizeof(vertices);
		bd.StructureByteStride = sizeof(Vertex);

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices;

		GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

		// bind vertex buffer to pipeline
		const UINT stride = sizeof(Vertex);
		const UINT offset = 0u;
		pContext->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offset);

		GFX_THROW_INFO_ONLY(pContext->Draw(3u, 0u));
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
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
};

