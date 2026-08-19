#include "DefenderMovementSystem.hpp"
#include <algorithm>

namespace game
{
	namespace
	{
		float	distanceAtX(const field::FieldPolyline& line, float x)
		{
			if (line.points.size() < 2 ||
				line.segmentLengths.size() + 1 != line.points.size() ||
				line.cumulativeLengths.size() != line.points.size())
				return 0.0f;
			if (x <= line.points.front().x)
				return 0.0f;
			if (x >= line.points.back().x)
				return line.totalLength;

			for (std::size_t index = 1; index < line.points.size(); ++index)
			{
				const field::Point2D&	start = line.points[index - 1];
				const field::Point2D&	end = line.points[index];

				if (x > end.x)
					continue;

				const float	deltaX = end.x - start.x;
				const float	progress = deltaX > 0.0f ? std::clamp((x - start.x) / deltaX, 0.0f, 1.0f) : 0.0f;

				return line.cumulativeLengths[index - 1] + line.segmentLengths[index - 1] * progress;
			}
			return line.totalLength;
		}

		float	moveToward(float current, float target, float maximumChange)
		{
			if (current < target)
				return std::min(current + maximumChange, target);
			return std::max(current - maximumChange, target);
		}
	}

	void	DefenderMovementSystem::update(
		World& world,
		const field::FieldLayout& field,
		engine::ecs::Entity player,
		float deltaTime
	) const
	{
		const Transform2D*	playerTransform = world.tryGetComponent<Transform2D>(player);

		if (playerTransform == nullptr)
			return;

		const field::Point2D	playerPosition{playerTransform->x, playerTransform->y};

		world.each<LineFollower, Transform2D>(
			[&field, playerPosition, deltaTime](
				engine::ecs::Entity,
				LineFollower& follower,
				Transform2D& transform
			)
			{
				const field::FieldPolyline*	line = field::tryGetLine(field, follower.lineId);

				if (line == nullptr || line->totalLength <= 0.0f)
					return;

				if (follower.lineId.kind == field::FieldLineKind::Longitudinal)
					follower.targetDistanceAlongLine = distanceAtX(*line, playerPosition.x);
				else
					follower.targetDistanceAlongLine = field::closestDistanceAlongLine(*line, playerPosition);

				follower.targetDistanceAlongLine = std::clamp(follower.targetDistanceAlongLine, 0.0f, line->totalLength);
				follower.distanceAlongLine = std::clamp(follower.distanceAlongLine, 0.0f, line->totalLength);
				follower.distanceAlongLine = moveToward(
					follower.distanceAlongLine,
					follower.targetDistanceAlongLine,
					std::max(0.0f, follower.moveSpeed * deltaTime)
				);

				const field::Point2D	position = field::pointAtDistance(*line, follower.distanceAlongLine);

				transform.x = position.x;
				transform.y = position.y;
			}
		);
	}
}
