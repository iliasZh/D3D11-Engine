#pragma once

#include "Bindable.h"

class VertexShader : public Bindable
{
public:
	VertexShader(Graphics& gfx, const std::wstring& csoFile)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		GFX_THROW_INFO(D3DReadFileToBlob(csoFile.c_str(), &pBlob));
		GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader
		(
			pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader
		));
	}
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
};