#include "DefenderCollisionSystem.hpp"
#include "game/GameConfig.hpp"
#include <algorithm>
#include <cstddef>

namespace game
{
	namespace
	{
		float	bumpDirection(
			float currentDistance,
			float otherDistance,
			engine::ecs::Entity entity,
			engine::ecs::Entity otherEntity
		)
		{
			if (currentDistance < otherDistance)
				return -1.0f;
			if (currentDistance > otherDistance)
				return 1.0f;
			return entity < otherEntity ? -1.0f : 1.0f;
		}

		void	bumpDefender(
			LineFollower& follower,
			Transform2D& transform,
			DefenderStatus& status,
			SprintState* sprint,
			const field::FieldPolyline& line,
			field::Point2D otherPosition,
			engine::ecs::Entity entity,
			engine::ecs::Entity otherEntity
		)
		{
			const float	currentDistance = std::clamp( follower.distanceAlongLine, 0.0f, line.totalLength);
			const float	otherDistance = field::closestDistanceAlongLine(line, otherPosition);
			const float	direction = bumpDirection(currentDistance, otherDistance, entity, otherEntity);
			float		bumpedDistance = std::clamp(
				currentDistance + direction * config::defender::collisionBumpDistance,
				0.0f,
				line.totalLength
			);

			if (bumpedDistance == currentDistance)
			{
				bumpedDistance = std::clamp(
					currentDistance - direction * config::defender::collisionBumpDistance,
					0.0f,
					line.totalLength
				);
			}

			follower.distanceAlongLine = bumpedDistance;
			follower.targetDistanceAlongLine = bumpedDistance;

			const field::Point2D	position = field::pointAtDistance(line, bumpedDistance);

			transform.x = position.x;
			transform.y = position.y;
			status.stunRemaining = std::max(status.stunRemaining, config::defender::collisionStunDuration);
			status.collisionCooldownRemaining = std::max(status.collisionCooldownRemaining, config::defender::collisionCooldown);
			if (sprint != nullptr)
			{
				sprint->requested = false;
				sprint->active = false;
			}
		}
	}

	void	DefenderCollisionSystem::update(
		World& world,
		const field::FieldLayout& field,
		const std::vector<engine::ecs::Entity>& defenders,
		const CollisionSystem& collisions
	) const
	{
		for (std::size_t firstIndex = 0; firstIndex < defenders.size(); ++firstIndex)
		{
			for (std::size_t secondIndex = firstIndex + 1; secondIndex < defenders.size(); ++secondIndex)
			{
				const engine::ecs::Entity	first = defenders[firstIndex];
				const engine::ecs::Entity	second = defenders[secondIndex];

				if (!collisions.hasCollision(first, second))
					continue;

				DefenderStatus*	firstStatus = world.tryGetComponent<DefenderStatus>(first);
				DefenderStatus*	secondStatus = world.tryGetComponent<DefenderStatus>(second);

				if (firstStatus == nullptr || secondStatus == nullptr || firstStatus->collisionCooldownRemaining > 0.0f ||
					secondStatus->collisionCooldownRemaining > 0.0f)
					continue;

				LineFollower*	firstFollower = world.tryGetComponent<LineFollower>(first);
				LineFollower*	secondFollower = world.tryGetComponent<LineFollower>(second);
				Transform2D*	firstTransform = world.tryGetComponent<Transform2D>(first);
				Transform2D*	secondTransform = world.tryGetComponent<Transform2D>(second);

				if (firstFollower == nullptr || secondFollower == nullptr || firstTransform == nullptr || secondTransform == nullptr)
					continue;

				const field::FieldPolyline*	firstLine = field::tryGetLine(field, firstFollower->lineId);
				const field::FieldPolyline*	secondLine = field::tryGetLine(field, secondFollower->lineId);

				if (firstLine == nullptr || secondLine == nullptr || firstLine->totalLength <= 0.0f || secondLine->totalLength <= 0.0f)
					continue;

				const field::Point2D	firstPosition{firstTransform->x, firstTransform->y};
				const field::Point2D	secondPosition{secondTransform->x, secondTransform->y};

				bumpDefender(
					*firstFollower,
					*firstTransform,
					*firstStatus,
					world.tryGetComponent<SprintState>(first),
					*firstLine,
					secondPosition,
					first,
					second
				);
				bumpDefender(
					*secondFollower,
					*secondTransform,
					*secondStatus,
					world.tryGetComponent<SprintState>(second),
					*secondLine,
					firstPosition,
					second,
					first
				);
			}
		}
	}
}
