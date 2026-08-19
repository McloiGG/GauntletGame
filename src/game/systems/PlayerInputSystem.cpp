#include "PlayerInputSystem.hpp"
#include <cmath>
#include "raylib.h"

namespace game
{
	void	PlayerInputSystem::update(World& world) const
	{
		world.each<PlayerController, Velocity2D>(
			[](
				engine::ecs::Entity,
				const PlayerController& controller,
				Velocity2D& velocity
			)
			{
				float	horizontal = 0.0f;
				float	vertical = 0.0f;

				if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
					horizontal -= 1.0f;
				if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
					horizontal += 1.0f;
				if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
					vertical -= 1.0f;
				if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
					vertical += 1.0f;

				const float lengthSquared = horizontal * horizontal + vertical * vertical;

				if (lengthSquared == 0.0f)
				{
					velocity = {};
					return;
				}

				const float	inverseLength = 1.0f / std::sqrt(lengthSquared);

				velocity.x = horizontal * inverseLength * controller.moveSpeed;
				velocity.y = vertical * inverseLength * controller.moveSpeed;
			}
		);
	}
}
