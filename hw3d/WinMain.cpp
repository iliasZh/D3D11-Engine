#include "Window.h"
#include "Utilities.h"
#include "App.h"

#pragma comment(lib, "XInput9_1_0")

int WINAPI wWinMain(
	_In_		HINSTANCE	hInstance,
	_In_opt_	HINSTANCE	hPrevInstance,
	_In_		PWSTR		CmdLine,
	_In_		int			nShowCmd)
{
	try
	{
		App app;

		return app.Go();
	}
	catch (const ChiliException& e)
	{
		SetWindowLongPtr(GetActiveWindow(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&DefWindowProc));
		MessageBox(nullptr, e.What(), e.GetType(), MB_OK | MB_ICONERROR);
	}
	catch (const std::exception& e)
	{
		SetWindowLongPtr(GetActiveWindow(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&DefWindowProc));
		MessageBox(nullptr, to_ws(e.what()).c_str(), L"Standard exception", MB_OK | MB_ICONERROR);
	}
	catch (...)
	{
		SetWindowLongPtr(GetActiveWindow(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&DefWindowProc));
		MessageBox(nullptr, L"No details available", L"Unknown exception", MB_OK | MB_ICONERROR);
	}
	return -1;
}