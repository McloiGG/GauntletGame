#include "engine/FrameClock.hpp"
#include "engine/VirtualScreen.hpp"
#include "engine/Window.hpp"
#include "raylib.h"

int main()
{
	constexpr int gameWidth = 640;
	constexpr int gameHeight = 360;
	engine::Window window{
		960,
		540,
		"Mini Gauntlet Engine Test",
		FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT
	};

	{
		engine::VirtualScreen virtualScreen{gameWidth, gameHeight};
		engine::FrameClock frameClock;

		while (!WindowShouldClose())
		{
			if (IsKeyPressed(KEY_F11))
				ToggleFullscreen();

			frameClock.update();
			virtualScreen.update();

			virtualScreen.beginGameDrawing();

			ClearBackground(Color{18, 24, 38, 255});

			// Corner markers reveal stretching or vertical flipping.
			DrawRectangle(0, 0, 32, 32, RED);
			DrawRectangle(gameWidth - 32, 0, 32, 32, GREEN);
			DrawRectangle(0, gameHeight - 32, 32, 32, BLUE);
			DrawRectangle(gameWidth - 32, gameHeight - 32, 32, 32, YELLOW);

			DrawLine(gameWidth / 2, 0, gameWidth / 2, gameHeight, GRAY);
			DrawLine(0, gameHeight / 2, gameWidth, gameHeight / 2, GRAY);
			DrawRectangleLines(0, 0, gameWidth - 1, gameHeight - 1, RED);
			DrawCircle(gameWidth / 2, gameHeight / 2, 24.0f, ORANGE);

			const Vector2 mousePosition =
				virtualScreen.screenToGame(GetMousePosition());
			if (mousePosition.x >= 0.0f && mousePosition.x < gameWidth
				&& mousePosition.y >= 0.0f && mousePosition.y < gameHeight)
			{
				DrawCircleV(mousePosition, 6.0f, PINK);
			}

			DrawText("F11: fullscreen | Resize the window", 10, 44, 16, WHITE);
			DrawText(
				TextFormat(
					"Window: %dx%d | Game: %dx%d",
					GetScreenWidth(),
					GetScreenHeight(),
					gameWidth,
					gameHeight
				),
				10,
				64,
				14,
				LIGHTGRAY
			);
			DrawText(
				TextFormat(
					"Delta: %.4f s | FPS: %d | Mouse: %.0f, %.0f",
					frameClock.deltaSeconds(),
					GetFPS(),
					mousePosition.x,
					mousePosition.y
				),
				10,
				82,
				14,
				LIGHTGRAY
			);

			virtualScreen.endGameDrawing();
		}
	}
}