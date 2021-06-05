#pragma once

#include "Bindable.h"

class PixelShader : public Bindable
{
public:
	PixelShader(Graphics& gfx, const std::wstring& csoFile)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		GFX_THROW_INFO(D3DReadFileToBlob(csoFile.c_str(), &pBlob));
		GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader
		(
			pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader
		));
	}
	void Bind(Graphics& gfx) override
	{
		GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u));
	}
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};