#pragma once
#ifndef DEFENDERSTATUS_HPP
#define DEFENDERSTATUS_HPP

namespace game
{
	enum class DefenderRecoveryOutcome
	{
		None,
		Stunned,
		NormalRecovery,
		FastRecovery,
		FullRecovery
	};

	struct DefenderStatus
	{
		DefenderRecoveryOutcome	recoveryOutcome{DefenderRecoveryOutcome::None};
		float					stunRemaining{};
		float					collisionCooldownRemaining{};
		bool					exhaustionOutcomeResolved{false};
	};
}

#endif
