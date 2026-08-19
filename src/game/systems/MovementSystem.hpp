#pragma once
#ifndef MOVEMENTSYSTEM_HPP
#define MOVEMENTSYSTEM_HPP

#include "game/World.hpp"

namespace game
{
	class MovementSystem
	{
	public:
		void	update(World& world, float deltaTime) const;
	};
}

#endif
