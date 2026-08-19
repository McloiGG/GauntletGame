#pragma once

#include <algorithm>
#include "raylib.h"

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
	int				p_gameW;
	int				p_gameH;
	RenderTexture2D	p_target;
	Rectangle		p_destination{};
	float 			p_scale{1.0f};
};
