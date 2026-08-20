#include "DefenderMovementSystem.hpp"
#include <algorithm>

namespace game
{
	namespace
	{
		float	moveToward(float current, float target, float maximumChange)
		{
			if (current < target)
				return std::min(current + maximumChange, target);
			return std::max(current - maximumChange, target);
		}
	}

	void	DefenderMovementSystem::update(World& world, const field::FieldLayout& field, float deltaTime) const
	{
		world.each<LineFollower, Transform2D>(
			[&world, &field, deltaTime](engine::ecs::Entity entity, LineFollower& follower, Transform2D& transform)
			{
				const field::FieldPolyline*	line = field::tryGetLine(field, follower.lineId);

				if (line == nullptr || line->totalLength <= 0.0f)
					return;

				follower.targetDistanceAlongLine = std::clamp(follower.targetDistanceAlongLine, 0.0f, line->totalLength);
				follower.distanceAlongLine = std::clamp(follower.distanceAlongLine, 0.0f, line->totalLength);

				const SprintState*		sprint = world.tryGetComponent<SprintState>(entity);
				const Stamina*			stamina = world.tryGetComponent<Stamina>(entity);
				const DefenderStatus*	status = world.tryGetComponent<DefenderStatus>(entity);
				float					speed = follower.walkSpeed;

				if (status != nullptr && status->stunRemaining > 0.0f)
					return;
				if (stamina != nullptr && stamina->exhausted)
					speed = follower.exhaustedSpeed;
				else if (sprint != nullptr && sprint->active)
					speed = follower.sprintSpeed;

				follower.distanceAlongLine = moveToward(
					follower.distanceAlongLine,
					follower.targetDistanceAlongLine,
					std::max(0.0f, speed * deltaTime)
				);

				const field::Point2D	position = field::pointAtDistance(*line, follower.distanceAlongLine);

				transform.x = position.x;
				transform.y = position.y;
			}
		);
	}
}
