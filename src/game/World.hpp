#pragma once
#ifndef WORLD_HPP
#define WORLD_HPP

#include "components/CircleCollider2D.hpp"
#include "components/DefenderStatus.hpp"
#include "components/LineFollower.hpp"
#include "components/MovementIntent2D.hpp"
#include "components/PlayerController.hpp"
#include "components/SprintState.hpp"
#include "components/Stamina.hpp"
#include "components/Transform2D.hpp"
#include "components/Velocity2D.hpp"
#include "engine/ecs/Registry.hpp"

namespace game
{
	using World = engine::ecs::Registry<
		Transform2D,
		Velocity2D,
		PlayerController,
		MovementIntent2D,
		SprintState,
		Stamina,
		CircleCollider2D,
		DefenderStatus,
		LineFollower
	>;
}

#endif
