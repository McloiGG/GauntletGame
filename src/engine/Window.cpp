#include "Window.hpp"
#include "raylib.h"

namespace engine
{
	Window::Window(int width, int height, const std::string& title, unsigned flags)
	{
		SetConfigFlags(flags);
		InitWindow(width, height, title.c_str());
	}

	Window::~Window()
	{
		CloseWindow();
	}
}
