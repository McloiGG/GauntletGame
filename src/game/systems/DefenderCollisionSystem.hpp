#pragma once
#ifndef DEFENDERCOLLISIONSYSTEM_HPP
#define DEFENDERCOLLISIONSYSTEM_HPP

#include "game/World.hpp"
#include "game/field/FieldLayout.hpp"
#include "game/systems/CollisionSystem.hpp"
#include <vector>

namespace game
{
	class DefenderCollisionSystem
	{
	public:
		void	update(
			World& world,
			const field::FieldLayout& field,
			const std::vector<engine::ecs::Entity>& defenders,
			const CollisionSystem& collisions
		) const;
	};
}

#endif
