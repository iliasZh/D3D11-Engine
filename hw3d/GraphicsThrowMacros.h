#pragma once

#define GFX_EXCEPT_NO_INFO(hr) Graphics::Exception{__LINE__, WFILE, (hr)}
#define GFX_THROW_NO_INFO(hrcall) if (FAILED(hr = (hrcall))) throw GFX_EXCEPT_NO_INFO(hr) 

#define INFOMAN_MSGS Graphics::GetInfoManager().GetMessages()
#define INFOMAN_SET Graphics::GetInfoManager().Set()

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::Exception{ __LINE__, WFILE, (hr), INFOMAN_MSGS }
#define GFX_THROW_INFO(hrcall) INFOMAN_SET; if ((hr = (hrcall)) != S_OK) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException{ __LINE__, WFILE, (hr), INFOMAN_MSGS }
#else // NDEBUG
#define GFX_EXCEPT(hr) Graphics::Exception{ __LINE__, WFILE, (hr) }
#define GFX_THROW_INFO(hrcall) GFX_THROW_NO_INFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException{ __LINE__, WFILE, (hr) }
#endif // NDEBUG