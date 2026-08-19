#include "FieldLayout.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

namespace game::field
{
	namespace
	{
		bool	hasValidMetrics(const FieldPolyline& polyline)
		{
			return polyline.segmentLengths.size() + 1 == polyline.points.size() &&
				polyline.cumulativeLengths.size() == polyline.points.size();
		}

		Point2D	interpolate(Point2D start, Point2D end, float progress)
		{
			return {
				start.x + (end.x - start.x) * progress,
				start.y + (end.y - start.y) * progress
			};
		}
	}

	bool	contains(const FieldRegion& region, float x, float y)
	{
		return x >= region.x && x <= region.x + region.width &&
			y >= region.y && y <= region.y + region.height;
	}

	SafeZone	findSafeZone(const FieldLayout& field, float x, float y)
	{
		if (contains(field.startSafeZone, x, y))
			return SafeZone::Start;
		if (contains(field.oppositeSafeZone, x, y))
			return SafeZone::Opposite;
		return SafeZone::None;
	}

	void	rebuildMetrics(FieldPolyline& polyline)
	{
		polyline.segmentLengths.clear();
		polyline.cumulativeLengths.clear();
		polyline.totalLength = 0.0f;
		if (polyline.points.empty())
			return;

		polyline.segmentLengths.reserve(polyline.points.size() - 1);
		polyline.cumulativeLengths.reserve(polyline.points.size());
		polyline.cumulativeLengths.push_back(0.0f);
		for (std::size_t index = 1; index < polyline.points.size(); ++index)
		{
			const Point2D&	start = polyline.points[index - 1];
			const Point2D&	end = polyline.points[index];
			const float	segmentLength = std::hypot(end.x - start.x, end.y - start.y);

			polyline.segmentLengths.push_back(segmentLength);
			polyline.totalLength += segmentLength;
			polyline.cumulativeLengths.push_back(polyline.totalLength);
		}
	}

	const FieldPolyline*	tryGetLine(const FieldLayout& field, FieldLineId lineId)
	{
		if (lineId.kind == FieldLineKind::Longitudinal)
		{
			if (lineId.index == 0)
				return &field.longitudinalDivider;
			return nullptr;
		}
		if (lineId.index >= field.transverseLines.size())
			return nullptr;
		return &field.transverseLines[lineId.index];
	}

	Point2D	pointAtDistance(const FieldPolyline& polyline, float distance)
	{
		if (polyline.points.empty())
			return {};
		if (polyline.points.size() == 1 || polyline.totalLength <= 0.0f ||
			!hasValidMetrics(polyline))
			return polyline.points.front();

		const float	targetDistance = std::clamp(distance, 0.0f, polyline.totalLength);

		for (std::size_t index = 1; index < polyline.points.size(); ++index)
		{
			const Point2D&	start = polyline.points[index - 1];
			const Point2D&	end = polyline.points[index];
			const float	segmentLength = polyline.segmentLengths[index - 1];
			const float	segmentStart = polyline.cumulativeLengths[index - 1];
			const float	segmentEnd = polyline.cumulativeLengths[index];

			if (targetDistance <= segmentEnd)
			{
				const float	segmentProgress = segmentLength > 0.0f ?
					(targetDistance - segmentStart) / segmentLength : 0.0f;

				return interpolate(start, end, segmentProgress);
			}
		}
		return polyline.points.back();
	}

	float	closestDistanceAlongLine(const FieldPolyline& polyline, Point2D point)
	{
		if (polyline.points.size() < 2 || !hasValidMetrics(polyline))
			return 0.0f;

		float	closestDistanceSquared = std::numeric_limits<float>::max();
		float	closestDistance = 0.0f;

		for (std::size_t index = 1; index < polyline.points.size(); ++index)
		{
			const Point2D&	start = polyline.points[index - 1];
			const Point2D&	end = polyline.points[index];
			const float	deltaX = end.x - start.x;
			const float	deltaY = end.y - start.y;
			const float	lengthSquared = deltaX * deltaX + deltaY * deltaY;
			float	segmentProgress = 0.0f;

			if (lengthSquared > 0.0f)
			{
				segmentProgress = ((point.x - start.x) * deltaX +
					(point.y - start.y) * deltaY) / lengthSquared;
				segmentProgress = std::clamp(segmentProgress, 0.0f, 1.0f);
			}

			const Point2D	closest = interpolate(start, end, segmentProgress);
			const float	distanceX = point.x - closest.x;
			const float	distanceY = point.y - closest.y;
			const float	distanceSquared = distanceX * distanceX + distanceY * distanceY;

			if (distanceSquared < closestDistanceSquared)
			{
				closestDistanceSquared = distanceSquared;
				closestDistance = polyline.cumulativeLengths[index - 1] +
					polyline.segmentLengths[index - 1] * segmentProgress;
			}
		}
		return closestDistance;
	}
}
