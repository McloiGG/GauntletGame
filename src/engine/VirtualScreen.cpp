#include "VirtualScreen.hpp"

namespace engine
{
	VirtualScreen::VirtualScreen(int gameWidth, int gameHeight)
		: m_gameW(gameWidth), m_gameH(gameHeight), m_target(LoadRenderTexture(gameWidth, gameHeight))
	{
	}
	VirtualScreen::~VirtualScreen() { UnloadRenderTexture(m_target); }

	void	VirtualScreen::update()
	{
		const float	windowWidth = static_cast<float>(GetScreenWidth());
		const float	windowHeight = static_cast<float>(GetScreenHeight());
		if (windowWidth <= 0.0f || windowHeight <= 0.0f)
			return;

		const float	horizontalScale = windowWidth / static_cast<float>(m_gameW);
		const float	verticalScale = windowHeight / static_cast<float>(m_gameH);

		m_scale = std::min(horizontalScale, verticalScale);

		const float scaledWidth = static_cast<float>(m_gameW) * m_scale;
		const float scaledHeight = static_cast<float>(m_gameH) * m_scale;

		m_destination = {
			(windowWidth - scaledWidth) * 0.5f,
			(windowHeight - scaledHeight) * 0.5f,
			scaledWidth,
			scaledHeight
		};
	}

	void	VirtualScreen::beginGameDrawing() const
	{
		BeginTextureMode(m_target);
		ClearBackground(BLACK);
	}

	void	VirtualScreen::endGameDrawing() const
	{
		EndTextureMode();

		const Rectangle	source{
			0.0f,
			0.0f,
			static_cast<float>(m_gameW),
			-static_cast<float>(m_gameH)
		};

		BeginDrawing();
		ClearBackground(BLACK);

		DrawTexturePro(
			m_target.texture,
			source,
			m_destination,
			Vector2{0.0f, 0.0f},
			0.0f,
			WHITE
		);

		EndDrawing();
	}

	Vector2	VirtualScreen::screenToGame(Vector2 screenPosition) const
	{
		return {
			(screenPosition.x - m_destination.x) / m_scale,
			(screenPosition.y - m_destination.y) / m_scale
		};
	}
}
