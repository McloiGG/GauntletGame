#pragma once
#ifndef STAMINA_HPP
#define STAMINA_HPP

namespace game
{
	struct Stamina
	{
		float	current{};
		float	maximum{};
		float	drainRate{};
		float	recoveryRate{};
		float	recoveryMultiplier{1.0f};
		float	recoveryDelay{};
		float	recoveryRemaining{};
		float	exhaustionRecoveryRatio{0.5f};
		bool	exhausted{false};
	};
}

#endif
