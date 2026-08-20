#pragma once
#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>

namespace engine
{
	class Window
	{
	public:
		Window(int width, int height, std::string title, unsigned flags);
		~Window() noexcept;
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(Window&&) = delete;

	private:
		std::string	m_title;
	};
}

#endif
