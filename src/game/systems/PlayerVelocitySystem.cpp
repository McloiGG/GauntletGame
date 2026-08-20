#include "PlayerVelocitySystem.hpp"

namespace game
{
	void	PlayerVelocitySystem::update(World& world) const
	{
		world.each<PlayerController, MovementIntent2D>(
			[&world](
				engine::ecs::Entity entity,
				const PlayerController& controller,
				const MovementIntent2D& intent
			)
			{
				Velocity2D*	velocity = world.tryGetComponent<Velocity2D>(entity);

				if (velocity == nullptr)
					return;

				const Stamina*		stamina = world.tryGetComponent<Stamina>(entity);
				const SprintState*	sprint = world.tryGetComponent<SprintState>(entity);
				float				speed = controller.walkSpeed;

				if (stamina != nullptr && stamina->exhausted)
					speed = controller.exhaustedSpeed;
				else if (sprint != nullptr && sprint->active)
					speed = controller.sprintSpeed;

				velocity->x = intent.x * speed;
				velocity->y = intent.y * speed;
			}
		);
	}
}
