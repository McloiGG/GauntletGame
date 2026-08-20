#include "Window.hpp"
#include "raylib.h"
#include <utility>

namespace engine
{
	Window::Window(int width, int height, std::string title, unsigned flags)
		: m_title(std::move(title))
	{
		if (m_title.empty() || m_title.front() == '\0')
			m_title = " ";
		SetConfigFlags(flags);
		InitWindow(width, height, m_title.c_str());
	}

	Window::~Window() noexcept
	{
		CloseWindow();
	}
}
