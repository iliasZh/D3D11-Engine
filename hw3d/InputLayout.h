#pragma once

#include "Bindable.h"

class InputLayout : public Bindable
{
public:
	InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
				ID3DBlob* pVertexShaderBytecode)
	{
		GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout
		(
			layout.data(), (UINT)layout.size(),
			pVertexShaderBytecode->GetBufferPointer(),
			pVertexShaderBytecode->GetBufferSize(),
			&pInputLayout
		));
	}
	void Bind(Graphics& gfx) override
	{
		GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
	}
private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};