#include "engine/FrameClock.hpp"
#include "engine/VirtualScreen.hpp"
#include "engine/Window.hpp"
#include "game/Game.hpp"
#include "game/GameConfig.hpp"
#include "game/field/FieldGenerator.hpp"
#include <cstdint>
#include <random>
#include "raylib.h"

int main()
{
	engine::Window	window{
		game::config::display::initialWindowWidth,
		game::config::display::initialWindowHeight,
		game::config::display::windowTitle,
		FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_FULLSCREEN_MODE
	};

	{
		engine::VirtualScreen	virtualScreen{
			game::config::display::logicalWidth,
			game::config::display::logicalHeight
		};
		engine::FrameClock			frameClock;
		game::field::FieldConfig	fieldConfig;

		fieldConfig.worldWidth = static_cast<float>(game::config::display::logicalWidth);
		fieldConfig.worldHeight = static_cast<float>(game::config::display::logicalHeight);

		std::random_device	randomDevice;
		const std::uint32_t	runSeed = static_cast<std::uint32_t>(randomDevice());
		game::Game			game{fieldConfig, runSeed};

		while (!WindowShouldClose())
		{
			frameClock.update();
			virtualScreen.update();
			if (game.isGameOver() && IsKeyPressed(KEY_ENTER))
			{
				const std::uint32_t	newSeed = static_cast<std::uint32_t>(randomDevice());

				game.startNewRun(newSeed);
			}

			game.update(frameClock.deltaSeconds());

			virtualScreen.beginGameDrawing();
			game.render();
			virtualScreen.endGameDrawing();
		}
	}
}
