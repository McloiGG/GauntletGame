#include "MovementSystem.hpp"

namespace game
{
	void	MovementSystem::update(World& world, float deltaTime) const
	{
		world.each<Velocity2D, Transform2D>(
			[deltaTime](
				engine::ecs::Entity,
				Velocity2D& velocity,
				Transform2D& transform
			)
			{
				transform.x += velocity.x * deltaTime;
				transform.y += velocity.y * deltaTime;
			}
		);
	}
}
