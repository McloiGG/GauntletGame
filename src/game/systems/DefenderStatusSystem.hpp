#pragma once
#ifndef DEFENDERSTATUSSYSTEM_HPP
#define DEFENDERSTATUSSYSTEM_HPP

#include "game/World.hpp"

namespace game
{
	class DefenderStatusSystem
	{
	public:
		void	update(World& world, float deltaTime) const;
	};
}

#endif
