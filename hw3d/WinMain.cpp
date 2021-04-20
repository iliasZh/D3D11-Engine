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