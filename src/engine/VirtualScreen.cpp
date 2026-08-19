#include "VirtualScreen.hpp"

VirtualScreen::VirtualScreen(int gameWidth, int gameHeight)
	: p_gameW(gameWidth), p_gameH(gameHeight), p_target(LoadRenderTexture(gameWidth, gameHeight))
{
}
VirtualScreen::~VirtualScreen() { UnloadRenderTexture(p_target); }

void	VirtualScreen::update()
{
	const float	windowWidth = static_cast<float>(GetScreenWidth());
	const float	windowHeight = static_cast<float>(GetScreenHeight());
	if (windowWidth <= 0.0f || windowHeight <= 0.0f)
		return;

	const float	horizontalScale = windowWidth / static_cast<float>(p_gameW);
	const float	verticalScale = windowHeight / static_cast<float>(p_gameH);

	p_scale = std::min(horizontalScale, verticalScale);

	const float scaledWidth = static_cast<float>(p_gameW) * p_scale;
	const float scaledHeight = static_cast<float>(p_gameH) * p_scale;

	p_destination = {
		(windowWidth - scaledWidth) * 0.5f,
		(windowHeight - scaledHeight) * 0.5f,
		scaledWidth,
		scaledHeight
	};
}

void	VirtualScreen::beginGameDrawing() const
{
	BeginTextureMode(p_target);
	ClearBackground(BLACK);
}

void	VirtualScreen::endGameDrawing() const
{
	EndTextureMode();

	const Rectangle	source{
		0.0f,
		0.0f,
		static_cast<float>(p_gameW),
		-static_cast<float>(p_gameH)
	};

	BeginDrawing();
	ClearBackground(BLACK);

	DrawTexturePro(
		p_target.texture,
		source,
		p_destination,
		Vector2{0.0f, 0.0f},
		0.0f,
		WHITE
	);

	EndDrawing();
}

Vector2	VirtualScreen::screenToGame(Vector2 screenPosition) const
{
	return {
		(screenPosition.x - p_destination.x) / p_scale,
		(screenPosition.y - p_destination.y) / p_scale
	};
}

