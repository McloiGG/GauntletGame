#include "FieldGenerator.hpp"
#include <algorithm>
#include <cmath>

namespace game::field
{
	namespace
	{
		bool	nearlyEqual(float first, float second)
		{
			return std::abs(first - second) <= game::config::field::comparisonTolerance;
		}

		bool	nearlyEqual(Point2D first, Point2D second)
		{
			return nearlyEqual(first.x, second.x) && nearlyEqual(first.y, second.y);
		}
	}

	FieldGenerator::FieldGenerator(std::uint32_t seed) : m_random(seed) {}

	FieldLayout	FieldGenerator::generate(const FieldConfig& config, std::uint32_t roundNumber, std::size_t transverseLineCount)
	{
		if (roundNumber == 1)
			return generateStraightField(config, transverseLineCount);

		for (std::size_t attempt = 0;
			attempt < game::config::field::generationAttempts;
			++attempt)
		{
			FieldLayout	field = generateCandidate(config, roundNumber, transverseLineCount);

			if (isValid(field, roundNumber))
				return field;
		}
		return generateStraightField(config, transverseLineCount);
	}

	FieldLayout	FieldGenerator::createBaseField(const FieldConfig& config) const
	{
		FieldLayout	field;

		field.courtBounds = {config.margin, config.margin,
			config.worldWidth - config.margin * 2.0f,
			config.worldHeight - config.margin * 2.0f};
		field.startSafeZone = {field.courtBounds.x, field.courtBounds.y,
			config.safeZoneWidth, field.courtBounds.height};
		field.oppositeSafeZone = {
			field.courtBounds.x + field.courtBounds.width - config.safeZoneWidth,
			field.courtBounds.y, config.safeZoneWidth, field.courtBounds.height
		};
		field.activeField = {field.startSafeZone.x + field.startSafeZone.width,
			field.courtBounds.y, field.courtBounds.width - config.safeZoneWidth * 2.0f,
			field.courtBounds.height};
		return field;
	}

	FieldLayout	FieldGenerator::generateStraightField(const FieldConfig& config, std::size_t transverseLineCount)
	{
		FieldLayout	field = createBaseField(config);
		const float	middleY = field.activeField.y + field.activeField.height * 0.5f;
		const float	slotWidth = field.activeField.width / static_cast<float>(transverseLineCount + 1);

		field.longitudinalDivider = generatePolyline({
			{field.activeField.x, middleY},
			{field.activeField.x + field.activeField.width, middleY},
			0,
			0.0f
		});
		field.transverseLines.reserve(transverseLineCount);
		for (std::size_t index = 0; index < transverseLineCount; ++index)
		{
			const float	x = field.activeField.x + slotWidth * static_cast<float>(index + 1);

			field.transverseLines.push_back(generatePolyline({
				{x, field.activeField.y},
				{x, field.activeField.y + field.activeField.height},
				0,
				0.0f
			}));
		}
		return field;
	}

	FieldLayout	FieldGenerator::generateCandidate(const FieldConfig& config, std::uint32_t roundNumber, std::size_t transverseLineCount)
	{
		FieldLayout	field = createBaseField(config);
		const float	slotWidth = field.activeField.width / static_cast<float>(transverseLineCount + 1);

		field.longitudinalDivider = generateLongitudinal(field.activeField, roundNumber);
		field.transverseLines.reserve(transverseLineCount);
		for (std::size_t index = 0; index < transverseLineCount; ++index)
		{
			const float	x = field.activeField.x + slotWidth * static_cast<float>(index + 1);

			field.transverseLines.push_back(generateTransverse(field.activeField, x, slotWidth, roundNumber));
		}
		return field;
	}

	FieldPolyline	FieldGenerator::generateLongitudinal(const FieldRegion& activeField, std::uint32_t roundNumber)
	{
		const float	middleY = activeField.y + activeField.height * 0.5f;

		return generatePolyline({
			{activeField.x, middleY},
			{activeField.x + activeField.width, middleY},
			chooseInteriorPointCount(roundNumber),
			longitudinalMaximumOffset(activeField, roundNumber)
		});
	}

	FieldPolyline	FieldGenerator::generateTransverse(const FieldRegion& activeField, float x, float slotWidth, std::uint32_t roundNumber)
	{
		return generatePolyline({
			{x, activeField.y},
			{x, activeField.y + activeField.height},
			chooseInteriorPointCount(roundNumber),
			transverseMaximumOffset(slotWidth)
		});
	}

	FieldPolyline	FieldGenerator::generatePolyline(const PolylineGenerationConfig& config)
	{
		FieldPolyline	line;
		const float	deltaX = config.end.x - config.start.x;
		const float	deltaY = config.end.y - config.start.y;
		const float	directLength = std::hypot(deltaX, deltaY);
		const float	normalX = directLength > 0.0f ? -deltaY / directLength : 0.0f;
		const float	normalY = directLength > 0.0f ? deltaX / directLength : 0.0f;

		line.points.reserve(config.interiorPointCount + 2);
		line.points.push_back(config.start);
		for (std::size_t index = 1; index <= config.interiorPointCount; ++index)
		{
			const float	progress = static_cast<float>(index) /
				static_cast<float>(config.interiorPointCount + 1);
			const float	offset = m_random.real(-config.maximumOffset, config.maximumOffset);

			line.points.push_back({
				config.start.x + deltaX * progress + normalX * offset,
				config.start.y + deltaY * progress + normalY * offset
			});
		}
		line.points.push_back(config.end);
		rebuildMetrics(line);
		return line;
	}

	std::size_t	FieldGenerator::chooseInteriorPointCount(std::uint32_t roundNumber)
	{
		const std::size_t	growth = roundNumber /
			game::config::field::roundsPerAdditionalInteriorPoint;
		const std::size_t	maximum = std::min(
			game::config::field::baseInteriorPointCount + growth,
			game::config::field::maximumInteriorPointCount
		);

		return static_cast<std::size_t>(m_random.integer(1, static_cast<int>(maximum)));
	}

	float	FieldGenerator::longitudinalMaximumOffset(
		const FieldRegion& activeField,
		std::uint32_t roundNumber
	) const
	{
		const std::uint32_t	completedRounds = roundNumber > 0 ? roundNumber - 1 : 0;
		const float	variationScale = std::max(0.0f, game::config::field::lineVariationScale);
		const float	progressionOffset =
			(game::config::field::longitudinalInitialOffset +
				static_cast<float>(completedRounds) *
				game::config::field::longitudinalOffsetIncreasePerRound) * variationScale;

		return std::min({
			progressionOffset,
			game::config::field::longitudinalMaximumOffset,
			activeField.height * game::config::field::longitudinalMaximumHeightRatio
		});
	}

	float	FieldGenerator::transverseMaximumOffset(float slotWidth) const
	{
		const float	variationScale = std::max(0.0f, game::config::field::lineVariationScale);

		return std::min(
			game::config::field::transverseMaximumOffset * variationScale,
			slotWidth * game::config::field::transverseMaximumSlotRatio
		);
	}

	bool	FieldGenerator::isValid(const FieldLayout& field, std::uint32_t roundNumber) const
	{
		const FieldRegion&	active = field.activeField;
		const FieldPolyline&	longitudinal = field.longitudinalDivider;

		if (active.width <= 0.0f || active.height <= 0.0f)
			return false;

		const float	middleY = active.y + active.height * 0.5f;

		if (!isValidPolyline(longitudinal, {
			{active.x, middleY},
			{active.x + active.width, middleY},
			active,
			longitudinalMaximumOffset(active, roundNumber)
		}))
			return false;

		const float	slotWidth = active.width / static_cast<float>(field.transverseLines.size() + 1);
		for (std::size_t lineIndex = 0; lineIndex < field.transverseLines.size(); ++lineIndex)
		{
			const FieldPolyline&	line = field.transverseLines[lineIndex];
			const float	x = active.x + slotWidth * static_cast<float>(lineIndex + 1);

			if (!isValidPolyline(line, {
				{x, active.y},
				{x, active.y + active.height},
				active,
				transverseMaximumOffset(slotWidth)
			}))
				return false;
		}
		return true;
	}

	bool	FieldGenerator::isValidPolyline(
		const FieldPolyline& line,
		const PolylineValidationConfig& config
	) const
	{
		if (line.points.size() < 2 || !nearlyEqual(line.points.front(), config.start) ||
			!nearlyEqual(line.points.back(), config.end))
			return false;

		const float	deltaX = config.end.x - config.start.x;
		const float	deltaY = config.end.y - config.start.y;
		const float	lengthSquared = deltaX * deltaX + deltaY * deltaY;
		const float	directLength = std::sqrt(lengthSquared);

		if (directLength <= 0.0f)
			return false;

		float	previousProgress = -1.0f;

		for (const Point2D& point : line.points)
		{
			if (!contains(config.bounds, point.x, point.y))
				return false;

			const float	fromStartX = point.x - config.start.x;
			const float	fromStartY = point.y - config.start.y;
			const float	progress = (fromStartX * deltaX + fromStartY * deltaY) / lengthSquared;
			const float	offset = std::abs(fromStartX * deltaY - fromStartY * deltaX) /
				directLength;

			if (progress <= previousProgress ||
				offset > config.maximumOffset + game::config::field::comparisonTolerance)
				return false;
			previousProgress = progress;
		}

		return true;
	}
}
