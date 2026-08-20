#pragma once
#ifndef DEFENDEREXHAUSTIONSYSTEM_HPP
#define DEFENDEREXHAUSTIONSYSTEM_HPP

#include "engine/Random.hpp"
#include "game/RoundDifficulty.hpp"
#include "game/World.hpp"

namespace game
{
	class DefenderExhaustionSystem
	{
	public:
		void	update(
			World& world,
			engine::Random& random,
			const RoundDifficulty& difficulty,
			float deltaTime
		) const;
	};
}

#endif
