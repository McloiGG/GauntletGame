#pragma once
#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <vector>
#include <string>

namespace engine
{
	class Window
	{
	public:
		Window(int width, int height, const std::string& title, unsigned flags);
		~Window();
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
	};
}

#endif
