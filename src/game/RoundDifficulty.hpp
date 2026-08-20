#pragma once
#ifndef ROUNDDIFFICULTY_HPP
#define ROUNDDIFFICULTY_HPP

#include "GameConfig.hpp"
#include <cstddef>

namespace game
{
	struct RoundDifficulty
	{
		std::size_t	transverseLineCount{config::difficulty::baseTransverseLineCount};
		float		enemySpeedMultiplier{config::difficulty::initialEnemySpeedMultiplier};
		float		enemyStaminaMultiplier{config::difficulty::initialEnemyStaminaMultiplier};
		float		enemyRecoveryMultiplier{config::difficulty::initialEnemyRecoveryMultiplier};
		float		enemyOutcomeWeightShift{config::difficulty::initialEnemyOutcomeWeightShift};
	};
}

#endif
