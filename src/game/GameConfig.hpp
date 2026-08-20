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
		inline constexpr char	windowTitle[] = "WEAVE";
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
		inline constexpr float	walkSpeed = 100.0f;
		inline constexpr float	sprintSpeed = 200.0f;
		inline constexpr float	exhaustedSpeed = 50.0f;
		inline constexpr float	colliderRadius = 12.0f;
		inline constexpr float	staminaMaximum = 100.0f;
		inline constexpr float	staminaDrainRate = 100.0f;
		inline constexpr float	staminaRecoveryRate = 30.0f;
		inline constexpr float	staminaRecoveryDelay = 0.6f;
		inline constexpr float	exhaustionRecoveryRatio = 0.5f;
	}

	namespace defender
	{
		inline constexpr float	walkSpeed = 50.0f;
		inline constexpr float	sprintSpeed = 100.0f;
		inline constexpr float	exhaustedSpeed = 32.0f;
		inline constexpr float	colliderRadius = 12.0f;
		inline constexpr float	collisionBumpDistance = 10.0f;
		inline constexpr float	collisionStunDuration = 0.45f;
		inline constexpr float	collisionCooldown = 1.5f;
		inline constexpr float	minimumSprintTravelDistance = 24.0f;
		inline constexpr float	longitudinalSprintProximity = 48.0f;
		inline constexpr float	transverseSprintProximity = 40.0f;
		inline constexpr float	staminaMaximum = 100.0f;
		inline constexpr float	staminaDrainRate = 32.0f;
		inline constexpr float	staminaRecoveryRate = 24.0f;
		inline constexpr float	staminaRecoveryDelay = 0.8f;
		inline constexpr float	exhaustionRecoveryRatio = 0.5f;
		inline constexpr float	exhaustionStunDuration = 1.0f;
		inline constexpr float	fastRecoveryMultiplier = 1.75f;
		inline constexpr float	stunnedOutcomeWeight = 20.0f;
		inline constexpr float	normalRecoveryOutcomeWeight = 50.0f;
		inline constexpr float	fastRecoveryOutcomeWeight = 25.0f;
		inline constexpr float	fullRecoveryOutcomeWeight = 5.0f;
		inline constexpr float	minimumStunnedOutcomeWeight = 5.0f;
		inline constexpr float	minimumNormalRecoveryOutcomeWeight = 10.0f;
		inline constexpr float	stunnedWeightShiftRatio = 0.6f;
		inline constexpr float	normalWeightShiftRatio = 0.4f;
		inline constexpr float	fastWeightGainRatio = 0.75f;
		inline constexpr float	fullWeightGainRatio = 0.25f;
		inline constexpr float	longitudinalSpawnProgress = 0.5f;
		inline constexpr float	firstTransverseSpawnProgress = 0.3f;
		inline constexpr float	secondTransverseSpawnProgress = 0.7f;
	}

	namespace hud
	{
		inline constexpr float	playerStaminaBarX = 78.0f;
		inline constexpr float	playerStaminaBarY = 72.0f;
		inline constexpr float	playerStaminaBarWidth = 120.0f;
		inline constexpr float	playerStaminaBarHeight = 8.0f;
		inline constexpr int	playerStaminaLabelX = 10;
		inline constexpr int	playerStaminaLabelY = 69;
		inline constexpr int	playerStaminaFontSize = 12;
		inline constexpr float	defenderStaminaBarWidth = 28.0f;
		inline constexpr float	defenderStaminaBarHeight = 4.0f;
		inline constexpr float	defenderStaminaBarGap = 5.0f;
		inline constexpr float	fullStaminaTolerance = 0.001f;
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
		inline constexpr float			initialEnemyRecoveryMultiplier = 1.0f;
		inline constexpr float			enemyRecoveryIncreasePerRound = 0.005f;
		inline constexpr float			initialEnemyOutcomeWeightShift = 0.0f;
		inline constexpr float			enemyOutcomeWeightShiftPerRound = 0.1f;
		inline constexpr float			maximumEnemyOutcomeWeightShift = 15.0f;
	}

	namespace random
	{
		inline constexpr std::uint32_t	enemyStream = 0x454E454Du;
	}

	namespace round
	{
		inline constexpr float	completeDelay = 1.0f;
		inline constexpr float	loadingDuration = 0.3f;
	}

	namespace persistence
	{
		inline constexpr char	highScorePath[] = "save/highscore.txt";
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
	static_assert(player::sprintSpeed > player::walkSpeed && player::walkSpeed > player::exhaustedSpeed,
		"Player sprint, walk, and exhausted speeds must be ordered fastest to slowest");
	static_assert(defender::sprintSpeed > defender::walkSpeed &&
		defender::walkSpeed > defender::exhaustedSpeed,
		"Defender sprint, walk, and exhausted speeds must be ordered fastest to slowest");
	static_assert(player::staminaMaximum > 0.0f && defender::staminaMaximum > 0.0f,
		"Stamina maximums must be positive");
	static_assert(player::staminaDrainRate >= 0.0f && player::staminaRecoveryRate >= 0.0f &&
		defender::staminaDrainRate >= 0.0f && defender::staminaRecoveryRate >= 0.0f,
		"Stamina rates cannot be negative");
	static_assert(player::exhaustionRecoveryRatio > 0.0f && player::exhaustionRecoveryRatio <= 1.0f &&
		defender::exhaustionRecoveryRatio > 0.0f && defender::exhaustionRecoveryRatio <= 1.0f,
		"Exhaustion recovery ratios must be greater than zero and at most one");
	static_assert(defender::longitudinalSprintProximity >= 0.0f &&
		defender::transverseSprintProximity >= 0.0f && defender::minimumSprintTravelDistance >= 0.0f,
		"Defender sprint distances cannot be negative");
	static_assert(defender::collisionBumpDistance >= 0.0f &&
		defender::collisionStunDuration >= 0.0f && defender::collisionCooldown >= 0.0f,
		"Defender collision response values cannot be negative");
	static_assert(defender::stunnedOutcomeWeight + defender::normalRecoveryOutcomeWeight +
		defender::fastRecoveryOutcomeWeight + defender::fullRecoveryOutcomeWeight == 100.0f,
		"Base defender recovery outcome weights must total 100 percent");
	static_assert(defender::fullRecoveryOutcomeWeight == 5.0f,
		"Base full-recovery outcome chance must remain five percent");
}

#endif
