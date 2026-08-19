#pragma once
#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include "game/World.hpp"
#include "game/collision/CollisionEvent.hpp"

#include <vector>

namespace game
{
	class CollisionSystem
	{
	public:
		void	update(const World& world);
		bool	hasCollision(engine::ecs::Entity first, engine::ecs::Entity second) const;

	private:
		std::vector<CollisionEvent>	m_events;
	};
}

#endif
