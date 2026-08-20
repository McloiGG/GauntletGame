#include "StaminaSystem.hpp"
#include <algorithm>

namespace game
{
	void	StaminaSystem::update(World& world, float deltaTime) const
	{
		const float	elapsed = std::max(0.0f, deltaTime);

		world.each<Stamina, SprintState>(
			[elapsed](engine::ecs::Entity, Stamina& stamina, SprintState& sprint)
			{
				stamina.maximum = std::max(0.0f, stamina.maximum);
				stamina.current = std::clamp(stamina.current, 0.0f, stamina.maximum);
				stamina.recoveryMultiplier = std::max(0.0f, stamina.recoveryMultiplier);
				stamina.recoveryRemaining = std::max(0.0f, stamina.recoveryRemaining);
				stamina.exhaustionRecoveryRatio = std::clamp(stamina.exhaustionRecoveryRatio, 0.0f, 1.0f);

				if (stamina.exhausted)
				{
					sprint.active = false;
					if (stamina.recoveryRemaining > 0.0f)
						stamina.recoveryRemaining = std::max(0.0f, stamina.recoveryRemaining - elapsed);
					else
						stamina.current = std::min(stamina.maximum,
							stamina.current + std::max(0.0f, stamina.recoveryRate) *
							stamina.recoveryMultiplier * elapsed);

					const float	recoveryThreshold = stamina.maximum * stamina.exhaustionRecoveryRatio;

					if (stamina.current >= recoveryThreshold)
					{
						stamina.exhausted = false;
						stamina.recoveryMultiplier = 1.0f;
					}
					return;
				}

				if (sprint.requested && stamina.current > 0.0f)
				{
					sprint.active = true;
					stamina.recoveryRemaining = std::max(0.0f, stamina.recoveryDelay);
					stamina.current = std::max(0.0f,
						stamina.current - std::max(0.0f, stamina.drainRate) * elapsed);

					if (stamina.current <= 0.0f)
					{
						sprint.active = false;
						stamina.exhausted = true;
					}
					return;
				}

				sprint.active = false;
				if (stamina.recoveryRemaining > 0.0f)
					stamina.recoveryRemaining = std::max(0.0f, stamina.recoveryRemaining - elapsed);
				else
					stamina.current = std::min(stamina.maximum,
						stamina.current + std::max(0.0f, stamina.recoveryRate) *
						stamina.recoveryMultiplier * elapsed);
			}
		);
	}
}
