#include "DefenderExhaustionSystem.hpp"
#include "game/GameConfig.hpp"
#include <algorithm>
#include <array>

namespace game
{
	namespace
	{
		std::array<float, 4>	recoveryOutcomeWeights(const RoundDifficulty& difficulty)
		{
			const float	shift = std::max(0.0f, difficulty.enemyOutcomeWeightShift);
			const float	stunnedWeight = std::max(config::defender::minimumStunnedOutcomeWeight,
				config::defender::stunnedOutcomeWeight - shift * config::defender::stunnedWeightShiftRatio);
			const float	normalWeight = std::max(config::defender::minimumNormalRecoveryOutcomeWeight,
				config::defender::normalRecoveryOutcomeWeight - shift * config::defender::normalWeightShiftRatio);
			const float	fastWeight = config::defender::fastRecoveryOutcomeWeight +
				shift * config::defender::fastWeightGainRatio;
			const float	fullWeight = config::defender::fullRecoveryOutcomeWeight +
				shift * config::defender::fullWeightGainRatio;

			return {stunnedWeight, normalWeight, fastWeight, fullWeight};
		}

		DefenderRecoveryOutcome	chooseRecoveryOutcome(engine::Random& random, const RoundDifficulty& difficulty)
		{
			switch (random.weightedIndex(recoveryOutcomeWeights(difficulty)))
			{
			case 0:
				return DefenderRecoveryOutcome::Stunned;
			case 1:
				return DefenderRecoveryOutcome::NormalRecovery;
			case 2:
				return DefenderRecoveryOutcome::FastRecovery;
			default:
				return DefenderRecoveryOutcome::FullRecovery;
			}
		}
	}

	void	DefenderExhaustionSystem::update(World& world, engine::Random& random, const RoundDifficulty& difficulty) const
	{
		world.each<Stamina, DefenderStatus>(
			[&random, &difficulty](engine::ecs::Entity, Stamina& stamina, DefenderStatus& status)
			{
				if (!stamina.exhausted)
				{
					status.exhaustionOutcomeResolved = false;
					status.recoveryOutcome = DefenderRecoveryOutcome::None;
					return;
				}
				if (status.exhaustionOutcomeResolved)
					return;

				status.recoveryOutcome = chooseRecoveryOutcome(random, difficulty);
				status.exhaustionOutcomeResolved = true;
				stamina.recoveryMultiplier = 1.0f;
				switch (status.recoveryOutcome)
				{
				case DefenderRecoveryOutcome::Stunned:
					status.stunRemaining = std::max(status.stunRemaining, config::defender::exhaustionStunDuration);
					break;
				case DefenderRecoveryOutcome::FastRecovery:
					stamina.recoveryMultiplier = config::defender::fastRecoveryMultiplier;
					break;
				case DefenderRecoveryOutcome::FullRecovery:
					stamina.current = stamina.maximum;
					stamina.recoveryRemaining = 0.0f;
					stamina.exhausted = false;
					break;
				case DefenderRecoveryOutcome::None:
				case DefenderRecoveryOutcome::NormalRecovery:
					break;
				}
			}
		);
	}
}
