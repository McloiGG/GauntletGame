#include "Intersections.hpp"

namespace engine::collision
{
	bool	intersects(const Circle& first, const Circle& second)
	{
		const float	differenceX = second.centerX - first.centerX;
		const float	differenceY = second.centerY - first.centerY;
		const float	combinedRadius = first.radius + second.radius;
		const float	distanceSquared = differenceX * differenceX + differenceY * differenceY;

		return distanceSquared <= combinedRadius * combinedRadius;
	}
}
