#pragma once

#include "Bindable.h"

class PrimitiveTopology : public Bindable
{
public:
	PrimitiveTopology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type) noexcept
		: type{ type }
	{}
	void Bind(Graphics& gfx) override
	{
		GFX_THROW_INFO_ONLY(GetContext(gfx)->IASetPrimitiveTopology(type));
	}
private:
	D3D11_PRIMITIVE_TOPOLOGY type;
};