#pragma once
#ifndef STAMINASYSTEM_HPP
#define STAMINASYSTEM_HPP

#include "game/World.hpp"

namespace game
{
	class StaminaSystem
	{
	public:
		void	update(World& world, float deltaTime) const;
	};
}

#endif
