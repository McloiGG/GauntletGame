#pragma once
#ifndef FIELDLAYOUT_HPP
#define FIELDLAYOUT_HPP

#include <cstddef>
#include <vector>

namespace game::field
{
	struct Point2D
	{
		float	x{};
		float	y{};
	};

	struct FieldRegion
	{
		float	x{};
		float	y{};
		float	width{};
		float	height{};
	};

	enum class FieldLineKind
	{
		Longitudinal,
		Transverse
	};

	struct FieldLineId
	{
		FieldLineKind	kind{FieldLineKind::Longitudinal};
		std::size_t		index{};
	};

	struct FieldPolyline
	{
		std::vector<Point2D>	points;
		std::vector<float>		segmentLengths;
		std::vector<float>		cumulativeLengths;
		float					totalLength{};
	};

	struct FieldLayout
	{
		FieldRegion				courtBounds;
		FieldRegion				activeField;
		FieldRegion				startSafeZone;
		FieldRegion				oppositeSafeZone;
		FieldPolyline				longitudinalDivider;
		std::vector<FieldPolyline>	transverseLines;
	};

	enum class SafeZone
	{
		None,
		Start,
		Opposite
	};

	bool	contains(const FieldRegion& region, float x, float y);
	SafeZone	findSafeZone(const FieldLayout& field, float x, float y);
	void	rebuildMetrics(FieldPolyline& polyline);
	const FieldPolyline*	tryGetLine(const FieldLayout& field, FieldLineId lineId);
	Point2D	pointAtDistance(const FieldPolyline& polyline, float distance);
	float	closestDistanceAlongLine(const FieldPolyline& polyline, Point2D point);
	float	distanceToLine(const FieldPolyline& polyline, Point2D point);
}

#endif
