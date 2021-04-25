#pragma once

#define CHWND_EXCEPT(hr) Window::HRException{ __LINE__, WFILE, (hr) }
#define CHWND_LAST_EXCEPT() Window::HRException{ __LINE__, WFILE, HRESULT_FROM_WIN32(GetLastError())}
#define CHWND_NO_GFX_EXCEPT() Window::NoGraphicsException{ __LINE__, WFILE }