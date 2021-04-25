#pragma once

#include "Window.h"
#include "ChiliTimer.h"

class App
{
public:
	App();
	int Go(); // returns exit code
private:
	void DoFrame();
private:
	Window wnd;
	ChiliTimer timer;
};