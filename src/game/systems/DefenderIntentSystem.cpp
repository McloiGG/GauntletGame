#include "DefenderIntentSystem.hpp"
#include "game/GameConfig.hpp"
#include <algorithm>
#include <cmath>

namespace game
{
	namespace
	{
		float	distanceAtX(const field::FieldPolyline& line, float x)
		{
			if (line.points.size() < 2 ||
				line.segmentLengths.size() + 1 != line.points.size() ||
				line.cumulativeLengths.size() != line.points.size() ||
				x <= line.points.front().x)
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
	}

	void	DefenderIntentSystem::update(
		World& world,
		const field::FieldLayout& field,
		engine::ecs::Entity player
	) const
	{
		const Transform2D*	playerTransform = world.tryGetComponent<Transform2D>(player);

		if (playerTransform == nullptr)
			return;

		const field::Point2D	playerPosition{playerTransform->x, playerTransform->y};

		world.each<LineFollower, SprintState>(
			[&world, &field, playerPosition](
				engine::ecs::Entity entity,
				LineFollower& follower,
				SprintState& sprint
			)
			{
				const field::FieldPolyline*	line = field::tryGetLine(field, follower.lineId);
				const DefenderStatus*		status = world.tryGetComponent<DefenderStatus>(entity);
				const Stamina*				stamina = world.tryGetComponent<Stamina>(entity);

				if (line == nullptr || line->totalLength <= 0.0f)
				{
					sprint.requested = false;
					return;
				}

				if (follower.lineId.kind == field::FieldLineKind::Longitudinal)
					follower.targetDistanceAlongLine = distanceAtX(*line, playerPosition.x);
				else
					follower.targetDistanceAlongLine = field::closestDistanceAlongLine(*line, playerPosition);

				follower.targetDistanceAlongLine = std::clamp(
					follower.targetDistanceAlongLine,
					0.0f,
					line->totalLength
				);

				const float	playerDistanceToLine = field::distanceToLine(*line, playerPosition);
				const float	travelDistance = std::abs(follower.targetDistanceAlongLine - follower.distanceAlongLine);
				const bool	isStunned = status != nullptr && status->stunRemaining > 0.0f;
				const bool	isExhausted = stamina != nullptr && stamina->exhausted;

				sprint.requested = !isStunned && !isExhausted &&
					playerDistanceToLine <= std::max(0.0f, follower.sprintPlayerProximity) &&
					travelDistance > config::defender::minimumSprintTravelDistance;
			}
		);
	}
}
