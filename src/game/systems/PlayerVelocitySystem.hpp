#pragma once
#ifndef PLAYERVELOCITYSYSTEM_HPP
#define PLAYERVELOCITYSYSTEM_HPP

#include "game/World.hpp"

namespace game
{
	class PlayerVelocitySystem
	{
	public:
		void	update(World& world) const;
	};
}

#endif
