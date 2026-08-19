#pragma once
#ifndef GAMECONFIG_HPP
#define GAMECONFIG_HPP

#include <cstddef>
#include <cstdint>

namespace game::config
{
	namespace display
	{
		inline constexpr int	logicalWidth = 640;
		inline constexpr int	logicalHeight = 360;
		inline constexpr int	initialWindowWidth = 960;
		inline constexpr int	initialWindowHeight = 540;
		inline constexpr char	windowTitle[] = "Mini Gauntlet Engine Test";
	}

	namespace field
	{
		inline constexpr float			margin = 24.0f;
		inline constexpr float			safeZoneWidth = 72.0f;
		inline constexpr std::size_t	generationAttempts = 8;
		inline constexpr float			comparisonTolerance = 0.001f;

		inline constexpr float			lineVariationScale = 1.0f;
		inline constexpr std::size_t	baseInteriorPointCount = 1;
		inline constexpr std::size_t	maximumInteriorPointCount = 5;
		inline constexpr std::uint32_t	roundsPerAdditionalInteriorPoint = 3;

		inline constexpr float	longitudinalInitialOffset = 18.0f;
		inline constexpr float	longitudinalOffsetIncreasePerRound = 1.5f;
		inline constexpr float	longitudinalMaximumOffset = 60.0f;
		inline constexpr float	longitudinalMaximumHeightRatio = 0.35f;
		inline constexpr float	transverseMaximumOffset = 16.0f;
		inline constexpr float	transverseMaximumSlotRatio = 0.25f;
	}

	namespace player
	{
		inline constexpr float	moveSpeed = 140.0f;
		inline constexpr float	colliderRadius = 12.0f;
	}

	namespace defender
	{
		inline constexpr float	moveSpeed = 48.0f;
		inline constexpr float	colliderRadius = 20.0f;
		inline constexpr float	longitudinalSpawnProgress = 0.5f;
		inline constexpr float	firstTransverseSpawnProgress = 0.3f;
		inline constexpr float	secondTransverseSpawnProgress = 0.7f;
	}

	namespace difficulty
	{
		inline constexpr std::size_t	baseTransverseLineCount = 2;
		inline constexpr std::size_t	maximumTransverseLineCount = 6;
		inline constexpr std::uint32_t	roundsPerMilestone = 5;
		inline constexpr float			initialEnemySpeedMultiplier = 1.0f;
		inline constexpr float			enemySpeedIncreasePerRound = 0.005f;
		inline constexpr float			initialEnemyStaminaMultiplier = 1.0f;
		inline constexpr float			enemyStaminaIncreasePerRound = 0.01f;
	}

	namespace round
	{
		inline constexpr float	completeDelay = 1.0f;
		inline constexpr float	loadingDuration = 0.3f;
	}

	static_assert(display::logicalWidth > 0 && display::logicalHeight > 0,
		"Logical display dimensions must be positive");
	static_assert(field::roundsPerAdditionalInteriorPoint > 0,
		"Field point growth interval must be greater than zero");
	static_assert(field::baseInteriorPointCount > 0 &&
		field::maximumInteriorPointCount >= field::baseInteriorPointCount,
		"Field interior point limits are invalid");
	static_assert(difficulty::roundsPerMilestone > 0,
		"Difficulty milestone interval must be greater than zero");
	static_assert(difficulty::maximumTransverseLineCount >=
		difficulty::baseTransverseLineCount,
		"Maximum transverse line count must include the base count");
	static_assert(defender::longitudinalSpawnProgress >= 0.0f &&
		defender::longitudinalSpawnProgress <= 1.0f &&
		defender::firstTransverseSpawnProgress >= 0.0f &&
		defender::firstTransverseSpawnProgress <= 1.0f &&
		defender::secondTransverseSpawnProgress >= 0.0f &&
		defender::secondTransverseSpawnProgress <= 1.0f,
		"Defender spawn progress values must be between zero and one");
}

#endif
