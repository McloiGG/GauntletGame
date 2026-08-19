#pragma once
#ifndef DEFENDERMOVEMENTSYSTEM_HPP
#define DEFENDERMOVEMENTSYSTEM_HPP

#include "game/World.hpp"
#include "game/field/FieldLayout.hpp"

namespace game
{
	class DefenderMovementSystem
	{
	public:
		void	update(
			World& world,
			const field::FieldLayout& field,
			engine::ecs::Entity player,
			float deltaTime
		) const;
	};
}

#endif
