#pragma once

#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& gfx, const std::vector<unsigned>& indices)
		: stride{ sizeof(unsigned) }
	{
		D3D11_BUFFER_DESC ibd = {};
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = stride * indices.size();
		ibd.StructureByteStride = stride;

		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = indices.data();
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&ibd, &isd, &pIndexBuffer));
	}
	void Bind(Graphics& gfx) override
	{
		DXGI_FORMAT format;
		if (stride == 2u)
			format = DXGI_FORMAT_R16_UINT;
		else if (stride == 4u)
			format = DXGI_FORMAT_R32_UINT;
		else
			throw std::logic_error("Use 16 or 32 bit integer format");

		GFX_THROW_INFO_ONLY(GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), format, 0u));
	}
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	UINT stride;
};