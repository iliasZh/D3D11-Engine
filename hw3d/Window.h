#pragma once

#include "ChiliWin.h"
#include "ChiliException.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Controller.h"
#include <optional>
#include <memory>
#include "WindowThrowMacros.h"

class Window
{
public:
	class Exception : public ChiliException
	{
	public:
		using ChiliException::ChiliException;
		static std::wstring TranslateErrorCode(HRESULT hr) noexcept;
	};
	class HRESULTException : public Exception
	{
	public:
		HRESULTException(int line, const wchar_t* file, HRESULT hr) noexcept;
		const wchar_t* What() const noexcept override;
		const wchar_t* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::wstring GetErrorString() const noexcept;
	private:
		HRESULT hr; 
	};
	class NoGraphicsException : public Exception
	{
	public:
		using Exception::Exception;
		const wchar_t* GetType() const noexcept override;
	};
private:
	class WindowClass
	{
	public:
		static const wchar_t* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const wchar_t* wndClassName = L"D3D11 Engine";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

public:
	Window(int width, int height, const wchar_t* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::wstring& title) const;
	static std::optional<int> ProcessMessages();
	Graphics& gfx();
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	Keyboard kbd;
	Mouse mouse;
	Controller controller;
private:
	HWND hWnd;
	int width;
	int height;
	std::unique_ptr<Graphics> pGfx;
};