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

}
