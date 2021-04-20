#include "App.h"

App::App()
    : wnd{ 800, 600, L"D3D11 Engine" }
{}

int App::Go()
{
    while (true)
    {
        if (const auto optional = Window::ProcessMessages())
        {
            return *optional;
        }
        DoFrame();
    }
}

void App::DoFrame()
{
    if (wnd.controller.IsConnected())
    {
        wss << L"left stick x: " << wnd.controller.LeftStickX()
            << L", left stick y: " << wnd.controller.LeftStickY();
        wnd.SetTitle(wss.str());
        wss.str(L"");
        wss.clear();
    }
    else
    {
        wnd.SetTitle(L"gamepad is disconnected");
    }
}
