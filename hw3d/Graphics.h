#pragma once

#pragma comment(lib, "D3DCompiler.lib")

#include "ChiliWin.h"
#include "ChiliException.h"
#include <d3d11.h>
#include "DXGIInfoManager.h"
#include "GraphicsThrowMacros.h"
#include <wrl.h>
#include <d3dcompiler.h>
#include "DirectXMath.h"

namespace dx = DirectX;

class Graphics
{
public:
	friend class Bindable;
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
	
	void DrawTestTriangle(float angle, float x, float z)
	{
		namespace wrl = Microsoft::WRL;

		struct ConstantBuffer
		{
			dx::XMMATRIX transform;
		};

		const ConstantBuffer cb =
		{
			dx::XMMatrixTranspose
			(
				dx::XMMatrixRotationZ(angle) *
				dx::XMMatrixRotationX(angle) *
				dx::XMMatrixTranslation(x, 0.0f, z) *
				dx::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.4f, 10.0f)
			)
		};

		wrl::ComPtr<ID3D11Buffer> pConstantBuffer;

		D3D11_BUFFER_DESC cbd = {};
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(cb);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &cb;

		GFX_THROW_INFO(pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));

		// bind constant buffer to vertex shader
		pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

		struct ConstantBuffer2
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[6];
		};
		const ConstantBuffer2 cb2 =
		{
			{
				{ 1.0f, 0.0f, 0.0f, 1.0f },
				{ 0.0f, 1.0f, 0.0f, 1.0f },
				{ 0.0f, 0.0f, 1.0f, 1.0f },
				{ 1.0f, 0.0f, 1.0f, 1.0f },
				{ 1.0f, 1.0f, 0.0f, 1.0f },
				{ 0.0f, 1.0f, 1.0f, 1.0f }
			}
		};

		wrl::ComPtr<ID3D11Buffer> pConstantBuffer2;

		D3D11_BUFFER_DESC cbd2 = {};
		cbd2.Usage = D3D11_USAGE_DYNAMIC;
		cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd2.MiscFlags = 0u;
		cbd2.ByteWidth = sizeof(cb2);
		cbd2.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd2 = {};
		csd2.pSysMem = &cb2;

		GFX_THROW_INFO(pDevice->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2));

		// bind constant buffer to the pixel shader
		pContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());

		struct Vertex
		{
			struct
			{
				float x;
				float y;
				float z;
			} pos;
			struct
			{
				unsigned char r;
				unsigned char g;
				unsigned char b;
				unsigned char a;
			} color;
		};


		float side = 0.5f;
		const Vertex vertices[] = 
		{
			{ { -side / 2.0f, +side / 2.0f, +side / 2.0f }, { 255u,   0u,   0u, 0u } },
			{ { +side / 2.0f, +side / 2.0f, +side / 2.0f }, {   0u, 255u,   0u, 0u } },
			{ { +side / 2.0f, +side / 2.0f, -side / 2.0f }, {   0u,   0u, 255u, 0u } },
			{ { -side / 2.0f, +side / 2.0f, -side / 2.0f }, { 255u,   0u, 255u, 0u } },
			{ { -side / 2.0f, -side / 2.0f, -side / 2.0f }, { 255u, 255u,   0u, 0u } },
			{ { -side / 2.0f, -side / 2.0f, +side / 2.0f }, {   0u, 255u, 255u, 0u } },
			{ { +side / 2.0f, -side / 2.0f, +side / 2.0f }, { 255u, 255u, 255u, 0u } },
			{ { +side / 2.0f, -side / 2.0f, -side / 2.0f }, {   0u,   0u,   0u, 0u } },
		};

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
		pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

		
		const unsigned int indices[] =
		{
			0u,1u,2u, 0u,2u,3u,	//    top face
			0u,6u,1u, 0u,5u,6u,	//   back face
			1u,7u,2u, 1u,6u,7u,	//  right face
			0u,3u,4u, 0u,4u,5u,	//   left face
			2u,4u,3u, 2u,7u,4u,	//  front face
			6u,5u,4u, 4u,7u,6u	// bottom face
		};

		wrl::ComPtr<ID3D11Buffer> pIndexBuffer;

		D3D11_BUFFER_DESC ibd = {};
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = sizeof(indices);
		ibd.StructureByteStride = sizeof(unsigned int);

		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = indices;
		GFX_THROW_INFO(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));

		// bind index buffer
		GFX_THROW_INFO_ONLY(pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u));



		// create pixel shader
		wrl::ComPtr<ID3D11PixelShader> pPixelShader;
		wrl::ComPtr<ID3DBlob> pBlob;
		GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
		GFX_THROW_INFO(pDevice->CreatePixelShader
		(
			pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader
		));

		// bind pixel shader
		GFX_THROW_INFO_ONLY(pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u));

		// create vertex shader
		wrl::ComPtr<ID3D11VertexShader> pVertexShader;
		GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
		GFX_THROW_INFO(pDevice->CreateVertexShader
		(
			pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader
		));

		// bind vertex shader
		GFX_THROW_INFO_ONLY(pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u));

		// input (vertex) layout
		wrl::ComPtr<ID3D11InputLayout> pInputLayout;
		const D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{ "POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u },
		};

		GFX_THROW_INFO(pDevice->CreateInputLayout
		(
			ied, (UINT)std::size(ied),
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			&pInputLayout
		));

		// bind input layout
		pContext->IASetInputLayout(pInputLayout.Get());

		//// bind render target
		//GFX_THROW_INFO_ONLY(pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr));

		// set primitive topology
		GFX_THROW_INFO_ONLY(pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));


		// draw
		GFX_THROW_INFO_ONLY(pContext->DrawIndexed(std::size(indices), 0u, 0u));
	}

#ifndef NDEBUG
	static DXGIInfoManager& GetInfoManager() noexcept
	{
		return infoMan;
	}
#endif
private:
#ifndef NDEBUG
	inline static DXGIInfoManager infoMan{};
#endif
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
};

