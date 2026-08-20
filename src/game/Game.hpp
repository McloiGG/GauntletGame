#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include "engine/Random.hpp"
#include "GameConfig.hpp"
#include "GamePhase.hpp"
#include "RoundDifficulty.hpp"
#include "RoundState.hpp"
#include "World.hpp"
#include "field/FieldGenerator.hpp"
#include "field/FieldLayout.hpp"
#include "persistence/HighScoreStore.hpp"
#include "systems/CollisionSystem.hpp"
#include "systems/DefenderExhaustionSystem.hpp"
#include "systems/DefenderIntentSystem.hpp"
#include "systems/DefenderMovementSystem.hpp"
#include "systems/MovementSystem.hpp"
#include "systems/PlayerInputSystem.hpp"
#include "systems/PlayerVelocitySystem.hpp"
#include "systems/StaminaSystem.hpp"
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
		bool	isGameOver() const;
		void	startNewRun(std::uint32_t seed);

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
		void	enterGameOver();
		void	renderPlayfield() const;
		void	renderHud() const;
		void	renderPlayerStaminaBar() const;
		void	renderDefenderStaminaBar(engine::ecs::Entity defender) const;
		void	renderLoading() const;
		void	renderRoundCompleteOverlay() const;
		void	renderGameOverOverlay() const;

		field::FieldConfig					m_fieldConfig;
		field::FieldGenerator				m_fieldGenerator;
		engine::Random						m_enemyRandom;
		persistence::HighScoreStore			m_highScoreStore{config::persistence::highScorePath};
		field::FieldLayout					m_field;
		World								m_world;
		engine::ecs::Entity					m_player{};
		std::vector<engine::ecs::Entity>	m_defenders;
		GamePhase							m_gamePhase{GamePhase::Playing};
		float								m_phaseTimer{};
		std::uint32_t						m_roundNumber{1};
		std::uint32_t						m_baseSeed{};
		std::uint32_t						m_completedRounds{};
		std::uint32_t						m_highScore{};
		RoundDifficulty						m_difficulty;
		RoundState							m_roundState{RoundState::GoingToOppositeSide};
		field::SafeZone						m_playerSafeZone{field::SafeZone::Start};
		bool								m_playerHit{false};
		bool								m_newHighScore{false};
		bool								m_highScoreSaveFailed{false};
		PlayerInputSystem					m_playerInputSystem;
		DefenderIntentSystem				m_defenderIntentSystem;
		StaminaSystem						m_staminaSystem;
		DefenderExhaustionSystem			m_defenderExhaustionSystem;
		PlayerVelocitySystem				m_playerVelocitySystem;
		MovementSystem						m_movementSystem;
		DefenderMovementSystem				m_defenderMovementSystem;
		CollisionSystem						m_collisionSystem;
	};
}

#endif
