#pragma once
#ifndef VIRTUALSCREEN_HPP
#define VIRTUALSCREEN_HPP

#include <algorithm>
#include "raylib.h"

namespace engine
{
	class VirtualScreen
	{
	public:
		VirtualScreen(int gameWidth, int gameHeight);
		~VirtualScreen();
		VirtualScreen(const VirtualScreen&) = delete;
		VirtualScreen& operator=(const VirtualScreen&) = delete;

		void	update();
		void	beginGameDrawing() const;
		void	endGameDrawing() const;
		Vector2	screenToGame(Vector2 screenPosition) const;

	private:
		int				m_gameW;
		int				m_gameH;
		RenderTexture2D	m_target;
		Rectangle		m_destination{};
		float 			m_scale{1.0f};
	};
}

#endif
