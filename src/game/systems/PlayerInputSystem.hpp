#pragma once
#ifndef PLAYERINPUTSYSTEM_HPP
#define PLAYERINPUTSYSTEM_HPP

#include "game/World.hpp"

namespace game
{
	class PlayerInputSystem
	{
	public:
		void	update(World& world) const;
	};
}

#endif
