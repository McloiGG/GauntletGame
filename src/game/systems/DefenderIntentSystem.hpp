#pragma once
#ifndef DEFENDERINTENTSYSTEM_HPP
#define DEFENDERINTENTSYSTEM_HPP

#include "game/World.hpp"
#include "game/field/FieldLayout.hpp"

namespace game
{
	class DefenderIntentSystem
	{
	public:
		void	update(World& world, const field::FieldLayout& field, engine::ecs::Entity player) const;
	};
}

#endif
