#pragma once

#include "Graphics.h"

class Bindable
{
public:
	virtual void Bind(Graphics& gfx) noexcept = 0;
	virtual ~Bindable() = default;
protected:
	static ID3D11DeviceContext* GetContext(Graphics& gfx) { return gfx.pContext.Get(); }
	static ID3D11Device* GetDevice(Graphics& gfx) { return gfx.pDevice.Get(); }
};