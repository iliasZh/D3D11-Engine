#include "App.h"

App::App()
    : wnd{ 800, 600, L"D3D11 Engine" }
{}

int App::Go()
{
    while (true)
    {
        // if optional is not empty, it will evaluate to true
        // ProcessMsgs returns non-empty optional only when it receives an exit msg
        if (const auto optional = Window::ProcessMessages())
        {
            return *optional;   // return exit code
        }
        DoFrame();  // draw stuff
    }
}

void App::DoFrame()
{
    wnd.gfx().ClearBuffer(0.0f, 0.0f, 0.0f);
    wnd.gfx().DrawTestTriangle();
    wnd.gfx().EndFrame();
}
