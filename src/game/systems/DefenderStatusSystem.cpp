#include "DefenderStatusSystem.hpp"
#include <algorithm>

namespace game
{
	void	DefenderStatusSystem::update(World& world, float deltaTime) const
	{
		const float	elapsed = std::max(0.0f, deltaTime);

		world.each<DefenderStatus, LineFollower>(
			[elapsed](engine::ecs::Entity, DefenderStatus& status, LineFollower&)
			{
				status.stunRemaining = std::max(0.0f, status.stunRemaining - elapsed);
				status.collisionCooldownRemaining = std::max(0.0f, status.collisionCooldownRemaining - elapsed);
			}
		);
	}
}
