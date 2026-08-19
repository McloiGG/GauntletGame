#pragma once
#ifndef COLLISIONEVENT_HPP
#define COLLISIONEVENT_HPP

#include "engine/ecs/Entity.hpp"

namespace game
{
	struct CollisionEvent
	{
		engine::ecs::Entity	first{};
		engine::ecs::Entity	second{};
	};
}

#endif
