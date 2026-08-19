#include "engine/VirtualScreen.hpp"
#include "raylib.h"

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(960, 540, "Virtual Screen Test");
	const int gameWidth = 640;
	const int gameHeight = 360;

	{
		VirtualScreen virtualScreen{gameWidth, gameHeight};

		while (!WindowShouldClose())
		{
			if (IsKeyPressed(KEY_F11))
				ToggleFullscreen();

			virtualScreen.update();

			virtualScreen.beginGameDrawing();

			ClearBackground(Color{18, 24, 38, 255});

			// If this becomes rectangular, scaling is distorted.
			DrawRectangle(
				gameWidth / 2 - 40,
				gameHeight / 2 - 40,
				80,
				80,
				SKYBLUE
			);

			// Shows the logical-screen boundary.
			DrawRectangleLines(
				0,
				0,
				gameWidth - 1,
				gameHeight - 1,
				RED
			);
			DrawText("Resize or press F11", 10, 10, 16, WHITE);

			virtualScreen.endGameDrawing();
		}
	}

    CloseWindow();
    return 0;
}