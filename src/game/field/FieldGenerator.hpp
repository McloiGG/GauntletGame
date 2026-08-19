#pragma once
#ifndef FIELDGENERATOR_HPP
#define FIELDGENERATOR_HPP

#include "FieldLayout.hpp"
#include "engine/Random.hpp"
#include "game/GameConfig.hpp"
#include <cstddef>
#include <cstdint>

namespace game::field
{
	struct FieldConfig
	{
		float		worldWidth{};
		float		worldHeight{};
		float		margin{game::config::field::margin};
		float		safeZoneWidth{game::config::field::safeZoneWidth};
	};

	class FieldGenerator
	{
	public:
		explicit FieldGenerator(std::uint32_t seed);

		FieldLayout	generate(const FieldConfig& config, std::uint32_t roundNumber, std::size_t transverseLineCount);

	private:
		struct PolylineGenerationConfig
		{
			Point2D		start;
			Point2D		end;
			std::size_t	interiorPointCount{};
			float		maximumOffset{};
		};

		struct PolylineValidationConfig
		{
			Point2D		start;
			Point2D		end;
			FieldRegion	bounds;
			float		maximumOffset{};
		};

		FieldLayout	createBaseField(const FieldConfig& config) const;
		FieldLayout	generateStraightField(const FieldConfig& config, std::size_t transverseLineCount);
		FieldLayout	generateCandidate(const FieldConfig& config, std::uint32_t roundNumber, std::size_t transverseLineCount);
		FieldPolyline	generateLongitudinal(const FieldRegion& activeField, std::uint32_t roundNumber);
		FieldPolyline	generateTransverse(const FieldRegion& activeField, float x, float slotWidth, std::uint32_t roundNumber);
		FieldPolyline	generatePolyline(const PolylineGenerationConfig& config);
		std::size_t	chooseInteriorPointCount(std::uint32_t roundNumber);
		float	longitudinalMaximumOffset(const FieldRegion& activeField, std::uint32_t roundNumber) const;
		float	transverseMaximumOffset(float slotWidth) const;
		bool	isValid(const FieldLayout& field, std::uint32_t roundNumber) const;
		bool	isValidPolyline(const FieldPolyline& line, const PolylineValidationConfig& config) const;

		engine::Random	m_random;
	};
}

#endif
