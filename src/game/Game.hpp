#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include "GamePhase.hpp"
#include "RoundDifficulty.hpp"
#include "RoundState.hpp"
#include "World.hpp"
#include "field/FieldGenerator.hpp"
#include "field/FieldLayout.hpp"
#include "systems/CollisionSystem.hpp"
#include "systems/DefenderMovementSystem.hpp"
#include "systems/MovementSystem.hpp"
#include "systems/PlayerInputSystem.hpp"
#include <cstdint>
#include <vector>

namespace game
{
	class Game
	{
	public:
		Game(field::FieldConfig fieldConfig, std::uint32_t baseSeed);

		void	update(float deltaTime);
		void	render() const;

	private:
		void	startRound();
		void	spawnDefender(field::FieldLineId lineId, float spawnProgress);
		void	updatePlaying(float deltaTime);
		void	updateRoundCompleteDelay(float deltaTime);
		void	updateLoadingNextRound(float deltaTime);
		bool	updatePhaseTimer(float deltaTime);
		void	clampPlayerToCourt();
		void	updateRoundProgress();
		void	updateCollisions();

		field::FieldConfig		m_fieldConfig;
		field::FieldGenerator	m_fieldGenerator;
		field::FieldLayout		m_field;
		World					m_world;
		engine::ecs::Entity		m_player{};
		std::vector<engine::ecs::Entity>	m_defenders;
		GamePhase				m_gamePhase{GamePhase::Playing};
		float					m_phaseTimer{};
		std::uint32_t			m_roundNumber{1};
		std::uint32_t			m_baseSeed{};
		RoundDifficulty			m_difficulty;
		RoundState				m_roundState{RoundState::GoingToOppositeSide};
		field::SafeZone			m_playerSafeZone{field::SafeZone::Start};
		bool					m_playerHit{false};
		PlayerInputSystem		m_playerInputSystem;
		MovementSystem			m_movementSystem;
		DefenderMovementSystem	m_defenderMovementSystem;
		CollisionSystem			m_collisionSystem;
	};
}

#endif
