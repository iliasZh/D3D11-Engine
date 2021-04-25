#include "Window.h"
#include <sstream>
#include "Resource.h"


/****************************EXCEPTIONS****************************/
/****************************EXCEPTIONS****************************/

Window::HRException::HRException(int line, const wchar_t* file, HRESULT hr) noexcept
	: Exception{ line, file }
	, hr{ hr }
{}

const wchar_t* Window::HRException::What() const noexcept
{
	std::wstringstream oss;
	oss << GetType() << std::endl << std::endl
		<< L"[Error code]: " << std::hex << std::showbase << GetErrorCode() << std::endl
		<< L"[Description]: " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const wchar_t* Window::HRException::GetType() const noexcept
{
	return L"Chili Window Exception";
}

std::wstring Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	wchar_t* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage
	(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&pMsgBuf), 0, nullptr
	);
	if (nMsgLen == 0)
	{
		return L"Unidentified error code";
	}
	std::wstring errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Window::HRException::GetErrorCode() const noexcept
{
	return hr;
}

std::wstring Window::HRException::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}

const wchar_t* Window::NoGraphicsException::GetType() const noexcept
{
	return L"Chili Window Exception: no graphics";
}

/****************************END*EXCEPTIONS****************************/
/****************************END*EXCEPTIONS****************************/


Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	: hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = wndClassName;
	wc.hIconSm = wc.hIcon;
	RegisterClassEx(&wc); 
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

const wchar_t* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

Window::Window(int width, int height, const wchar_t* name)
	: width{ width }
	, height{ height }
{
	// calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == FALSE)
	{
		throw CHWND_LAST_EXCEPT();
	}

	// create window and get a handle
	hWnd = CreateWindow
	(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);

	if (hWnd == nullptr)
	{
		throw CHWND_LAST_EXCEPT();
	}

	ShowWindow(hWnd, SW_SHOWDEFAULT);

	pGfx = std::make_unique<Graphics>(hWnd);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

void Window::SetTitle(const std::wstring& title) const
{
	if (SetWindowText(hWnd, title.c_str()) == FALSE)
	{
		throw CHWND_LAST_EXCEPT();
	}
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// when quit message is received, return exit code
		if (msg.message == WM_QUIT)
		{
			return std::optional<int>{ msg.wParam };
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg); // dispatch to HandleMsg
	}

	// if all messages are processed but there wasn't a quit message, return empty optional
	return std::optional<int>{};
}

Graphics& Window::gfx()
{
	if (!pGfx)
	{
		throw CHWND_NO_GFX_EXCEPT();
	}
	return *pGfx;
}

// hanky panky windows magic
LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

// main message handling function
LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_KILLFOCUS:
		// release all keys when window is out of focus
		kbd.ClearKeyStates();
		break;

	//***************************KEYBOARD*MESSAGES***************************
	//***************************KEYBOARD*MESSAGES***************************
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		// KF_REPEAT's bit is 1 if autorepeat's working (key was down before current msg is sent)
		if (!(lParam & KF_REPEAT) || kbd.IsAutorepeatEnabled())
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<char>(wParam));
		break;
	//***********************END*KEYBOARD*MESSAGES***************************
	//***********************END*KEYBOARD*MESSAGES***************************


	//***************************MOUSE*MESSAGES***************************
	//***************************MOUSE*MESSAGES***************************
	case WM_MOUSEMOVE:
		const POINTS p = MAKEPOINTS(lParam);
		if (p.x >= 0 && p.x < width && p.y >= 0 && p.y < height)
		{
			mouse.OnMove(p.x, p.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnEnter();
			}
		}
		else
		{
			if (mouse.IsLeftPressed() || mouse.IsRightPressed() || mouse.IsMiddlePressed())
			{
				mouse.OnMove(p.x, p.y);
			}
			else
			{
				ReleaseCapture();
				mouse.OnLeave();
			}
		}
		break;
	case WM_LBUTTONDOWN:
		mouse.OnLeftPressed();
		break;
	case WM_LBUTTONUP:
		mouse.OnLeftReleased();
		break;
	case WM_MBUTTONDOWN:
		mouse.OnMiddlePressed();
		break;
	case WM_MBUTTONUP:
		mouse.OnMiddleReleased();
		break;
	case WM_RBUTTONDOWN:
		mouse.OnRightPressed();
		break;
	case WM_RBUTTONUP:
		mouse.OnRightReleased();
		break;
	case WM_MOUSEWHEEL:
		auto wd = GET_WHEEL_DELTA_WPARAM(wParam);
		while (wd >= WHEEL_DELTA)
		{
			wd -= WHEEL_DELTA;
			mouse.OnWheelUp();
		}
		while (wd <= -WHEEL_DELTA)
		{
			wd += WHEEL_DELTA;
			mouse.OnWheelDown();
		}
		break;
	//***********************END*MOUSE*MESSAGES***************************
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}