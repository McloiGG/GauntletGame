#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include "World.hpp"
#include "systems/CollisionSystem.hpp"
#include "systems/MovementSystem.hpp"
#include "systems/PlayerInputSystem.hpp"

namespace game
{
	class Game
	{
	public:
		Game(float worldWidth, float worldHeight);

		void	update(float deltaTime);
		void	render() const;

	private:
		World					m_world;
		engine::ecs::Entity		m_player{};
		engine::ecs::Entity		m_defender{};
		bool					m_playerHit{false};
		PlayerInputSystem		m_playerInputSystem;
		MovementSystem			m_movementSystem;
		CollisionSystem			m_collisionSystem;
	};
}

#endif
