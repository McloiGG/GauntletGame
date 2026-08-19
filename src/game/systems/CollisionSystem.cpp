#include "CollisionSystem.hpp"
#include "engine/collision/Circle.hpp"
#include "engine/collision/Intersections.hpp"

namespace game
{
	namespace
	{
		engine::collision::Circle	toCircle(const Transform2D& transform, const CircleCollider2D& collider)
		{
			return {transform.x, transform.y, collider.radius};
		}
	}

	void	CollisionSystem::update(const World& world)
	{
		m_events.clear();

		world.each<CircleCollider2D, Transform2D>(
			[this, &world](
				engine::ecs::Entity firstEntity,
				const CircleCollider2D& firstCollider,
				const Transform2D& firstTransform
			)
			{
				world.each<CircleCollider2D, Transform2D>(
					[this, firstEntity, &firstCollider, &firstTransform](
						engine::ecs::Entity secondEntity,
						const CircleCollider2D& secondCollider,
						const Transform2D& secondTransform
					)
					{
						if (firstEntity >= secondEntity)
							return;

						const engine::collision::Circle	firstCircle = toCircle(firstTransform, firstCollider);
						const engine::collision::Circle	secondCircle = toCircle(secondTransform, secondCollider);

						if (engine::collision::intersects(firstCircle, secondCircle))
							m_events.push_back(CollisionEvent{firstEntity, secondEntity});
					}
				);
			}
		);
	}

	bool	CollisionSystem::hasCollision(engine::ecs::Entity first, engine::ecs::Entity second) const
	{
		for (const CollisionEvent& event : m_events)
		{
			if ((event.first == first && event.second == second) ||
				(event.first == second && event.second == first))
				return true;
		}
		return false;
	}
}
