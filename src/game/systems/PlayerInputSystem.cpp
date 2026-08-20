#include "PlayerInputSystem.hpp"
#include <cmath>
#include "raylib.h"

namespace game
{
	void	PlayerInputSystem::update(World& world) const
	{
		world.each<MovementIntent2D, SprintState>(
			[](engine::ecs::Entity, MovementIntent2D& intent, SprintState& sprint)
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

				const float	lengthSquared = horizontal * horizontal + vertical * vertical;

				if (lengthSquared == 0.0f)
				{
					intent = {};
					sprint.requested = false;
					return;
				}

				const float	inverseLength = 1.0f / std::sqrt(lengthSquared);

				intent.x = horizontal * inverseLength;
				intent.y = vertical * inverseLength;
				sprint.requested = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
			}
		);
	}
}
