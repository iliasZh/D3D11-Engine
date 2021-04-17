#include "Window.h"
#include "Utilities.h"

int WINAPI wWinMain(
	_In_		HINSTANCE	hInstance,
	_In_opt_	HINSTANCE	hPrevInstance,
	_In_		PWSTR		CmdLine,
	_In_		int			nShowCmd)
{
	try
	{
		Window wnd(800, 600, L"D3D11 Engine");

		MSG msg;
		BOOL gResult;

		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (gResult == -1)
		{
			return -1;
		}

		return msg.wParam;
	}
	catch (const ChiliException& e)
	{
		MessageBox(nullptr, e.What(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, to_ws(e.what()).c_str(), L"Standard exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, L"No details available", L"Unknown exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}