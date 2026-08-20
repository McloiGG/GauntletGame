#include "Game.hpp"
#include "GameConfig.hpp"
#include <algorithm>
#include <cstddef>
#include <utility>
#include "raylib.h"

namespace game
{
	namespace
	{
		Rectangle	toRectangle(const field::FieldRegion& region)
		{
			return {region.x, region.y, region.width, region.height};
		}

		const char*	objectiveText(RoundState state)
		{
			if (state == RoundState::GoingToOppositeSide)
				return "Reach Safe B";
			if (state == RoundState::ReturningToStart)
				return "Return to Safe A";
			return "Round complete";
		}

		RoundDifficulty	difficultyForRound(std::uint32_t roundNumber)
		{
			const std::uint32_t	completedRounds = roundNumber > 0 ? roundNumber - 1 : 0;
			const std::size_t	milestoneCount = roundNumber / config::difficulty::roundsPerMilestone;
			RoundDifficulty		difficulty;

			difficulty.transverseLineCount = std::min(
				config::difficulty::baseTransverseLineCount + milestoneCount,
				config::difficulty::maximumTransverseLineCount
			);
			difficulty.enemySpeedMultiplier =
				config::difficulty::initialEnemySpeedMultiplier +
				static_cast<float>(completedRounds) *
				config::difficulty::enemySpeedIncreasePerRound;
			difficulty.enemyStaminaMultiplier =
				config::difficulty::initialEnemyStaminaMultiplier +
				static_cast<float>(completedRounds) *
				config::difficulty::enemyStaminaIncreasePerRound;
			return difficulty;
		}

		void	drawPolyline(const field::FieldPolyline& polyline, Color color)
		{
			for (std::size_t index = 1; index < polyline.points.size(); ++index)
			{
				const field::Point2D&	start = polyline.points[index - 1];
				const field::Point2D&	end = polyline.points[index];

				DrawLineV(Vector2{start.x, start.y}, Vector2{end.x, end.y}, color);
			}
		}

		void	drawCenteredText(const char* text, int y, int fontSize, Color color, float width)
		{
			const int	x = static_cast<int>((width - static_cast<float>(MeasureText(text, fontSize))) * 0.5f);

			DrawText(text, x, y, fontSize, color);
		}

		bool	drawEntityCircle(
			const World& world,
			engine::ecs::Entity entity,
			Color color
		)
		{
			const Transform2D*		transform = world.tryGetComponent<Transform2D>(entity);
			const CircleCollider2D*	collider = world.tryGetComponent<CircleCollider2D>(entity);

			if (transform == nullptr || collider == nullptr)
				return false;

			DrawCircleV(Vector2{transform->x, transform->y}, collider->radius, color);
			return true;
		}
	}

	Game::Game(field::FieldConfig fieldConfig, std::uint32_t baseSeed)
		: m_fieldConfig(fieldConfig), m_fieldGenerator(baseSeed), m_baseSeed(baseSeed)
	{
		m_highScore = m_highScoreStore.load();
		startRound();
	}

	bool	Game::isGameOver() const
	{
		return m_gamePhase == GamePhase::GameOver;
	}

	void	Game::startNewRun(std::uint32_t seed)
	{
		m_baseSeed = seed;
		m_fieldGenerator = field::FieldGenerator{seed};
		m_roundNumber = 1;
		m_completedRounds = 0;
		m_phaseTimer = config::round::loadingDuration;
		m_gamePhase = GamePhase::LoadingNextRound;
		m_playerHit = false;
		m_newHighScore = false;
		m_highScoreSaveFailed = false;
		startRound();
	}

	void	Game::startRound()
	{
		m_difficulty = difficultyForRound(m_roundNumber);
		m_field = m_fieldGenerator.generate(m_fieldConfig, m_roundNumber, m_difficulty.transverseLineCount);
		m_world = World{};
		m_defenders.clear();
		m_collisionSystem = CollisionSystem{};
		m_player = m_world.createEntity();

		const field::FieldRegion&	start = m_field.startSafeZone;
		const float					playerX = start.x + start.width * 0.5f;
		const float					playerY = start.y + start.height * 0.5f;

		m_world.addComponent(m_player, Transform2D{playerX, playerY});
		m_world.addComponent(m_player, Velocity2D{});
		m_world.addComponent(m_player, CircleCollider2D{config::player::colliderRadius});
		m_world.addComponent(m_player, PlayerController{config::player::moveSpeed});
		spawnDefender(
			{field::FieldLineKind::Longitudinal, 0},
			config::defender::longitudinalSpawnProgress
		);
		for (std::size_t index = 0; index < m_field.transverseLines.size(); ++index)
		{
			const float	spawnProgress = index % 2 == 0 ?
				config::defender::firstTransverseSpawnProgress :
				config::defender::secondTransverseSpawnProgress;

			spawnDefender({field::FieldLineKind::Transverse, index}, spawnProgress);
		}

		m_roundState = RoundState::GoingToOppositeSide;
		m_playerSafeZone = field::findSafeZone(m_field, playerX, playerY);
		m_playerHit = false;
	}

	void	Game::spawnDefender(field::FieldLineId lineId, float spawnProgress)
	{
		const field::FieldPolyline*	line = field::tryGetLine(m_field, lineId);

		if (line == nullptr || line->points.size() < 2 || line->totalLength <= 0.0f)
			return;

		const float					distance = line->totalLength * std::clamp(spawnProgress, 0.0f, 1.0f);
		const field::Point2D		position = field::pointAtDistance(*line, distance);
		const engine::ecs::Entity	defender = m_world.createEntity();

		m_world.addComponent(defender, Transform2D{position.x, position.y});
		m_world.addComponent(defender, CircleCollider2D{config::defender::colliderRadius});
		m_world.addComponent(defender, LineFollower{
			lineId,
			distance,
			distance,
			config::defender::moveSpeed * m_difficulty.enemySpeedMultiplier
		});
		m_defenders.push_back(defender);
	}

	void	Game::update(float deltaTime)
	{
		switch (m_gamePhase)
		{
		case GamePhase::Playing:
			updatePlaying(deltaTime);
			break;
		case GamePhase::RoundCompleteDelay:
			updateRoundCompleteDelay(deltaTime);
			break;
		case GamePhase::LoadingNextRound:
			updateLoadingNextRound(deltaTime);
			break;
		case GamePhase::GameOver:
			break;
		}
	}

	void	Game::updatePlaying(float deltaTime)
	{
		m_playerInputSystem.update(m_world);
		m_movementSystem.update(m_world, deltaTime);
		clampPlayerToCourt();
		m_defenderMovementSystem.update(m_world, m_field, m_player, deltaTime);
		updateRoundProgress();

		if (m_roundState == RoundState::Complete)
		{
			Velocity2D*	velocity = m_world.tryGetComponent<Velocity2D>(m_player);

			if (velocity != nullptr)
				*velocity = {};
			++m_completedRounds;
			m_playerHit = false;
			m_gamePhase = GamePhase::RoundCompleteDelay;
			m_phaseTimer = config::round::completeDelay;
			return;
		}
		updateCollisions();
		if (m_playerHit)
			enterGameOver();
	}

	void	Game::updateRoundCompleteDelay(float deltaTime)
	{
		if (!updatePhaseTimer(deltaTime))
			return;

		++m_roundNumber;
		startRound();
		m_gamePhase = GamePhase::LoadingNextRound;
		m_phaseTimer = config::round::loadingDuration;
	}

	void	Game::updateLoadingNextRound(float deltaTime)
	{
		if (updatePhaseTimer(deltaTime))
			m_gamePhase = GamePhase::Playing;
	}

	bool	Game::updatePhaseTimer(float deltaTime)
	{
		m_phaseTimer = std::max(0.0f, m_phaseTimer - deltaTime);

		return m_phaseTimer <= 0.0f;
	}

	void	Game::clampPlayerToCourt()
	{
		Transform2D*		transform = m_world.tryGetComponent<Transform2D>(m_player);
		CircleCollider2D*	collider = m_world.tryGetComponent<CircleCollider2D>(m_player);

		if (transform == nullptr || collider == nullptr)
			return;

		const field::FieldRegion&	court = m_field.courtBounds;

		transform->x = std::clamp(transform->x, court.x + collider->radius, court.x + court.width - collider->radius);
		transform->y = std::clamp(transform->y, court.y + collider->radius, court.y + court.height - collider->radius);
	}

	void	Game::updateRoundProgress()
	{
		const Transform2D*	transform = m_world.tryGetComponent<Transform2D>(m_player);

		if (transform == nullptr)
			return;

		m_playerSafeZone = field::findSafeZone(m_field, transform->x, transform->y);
		if (m_roundState == RoundState::GoingToOppositeSide && m_playerSafeZone == field::SafeZone::Opposite)
			m_roundState = RoundState::ReturningToStart;
		else if (m_roundState == RoundState::ReturningToStart && m_playerSafeZone == field::SafeZone::Start)
			m_roundState = RoundState::Complete;
	}

	void	Game::updateCollisions()
	{
		m_collisionSystem.update(m_world);
		m_playerHit = false;
		if (m_playerSafeZone != field::SafeZone::None)
			return;

		for (const engine::ecs::Entity defender : m_defenders)
		{
			if (m_collisionSystem.hasCollision(m_player, defender))
			{
				m_playerHit = true;
				return;
			}
		}
	}

	void	Game::enterGameOver()
	{
		Velocity2D*	velocity = m_world.tryGetComponent<Velocity2D>(m_player);

		if (velocity != nullptr)
			*velocity = {};

		m_newHighScore = m_completedRounds > m_highScore;
		m_highScoreSaveFailed = false;
		if (m_newHighScore)
		{
			m_highScore = m_completedRounds;
			m_highScoreSaveFailed = !m_highScoreStore.save(m_highScore);
		}
		m_gamePhase = GamePhase::GameOver;
	}

	void	Game::render() const
	{
		if (m_gamePhase == GamePhase::LoadingNextRound)
		{
			ClearBackground(BLACK);
			drawCenteredText(TextFormat("LOADING ROUND %u...", static_cast<unsigned int>(m_roundNumber)),
				130, 28, RAYWHITE, m_fieldConfig.worldWidth);
			drawCenteredText(TextFormat("RUN SEED: %u", static_cast<unsigned int>(m_baseSeed)),
				166, 16, LIGHTGRAY, m_fieldConfig.worldWidth);
			drawCenteredText(TextFormat("ENEMIES: %u | SPEED x%.3f | STAMINA x%.3f",
				static_cast<unsigned int>(m_defenders.size()), m_difficulty.enemySpeedMultiplier,
				m_difficulty.enemyStaminaMultiplier), 190, 14, GRAY, m_fieldConfig.worldWidth);
			return;
		}

		DrawRectangleRec(toRectangle(m_field.courtBounds), Color{35, 66, 52, 255});
		DrawRectangleRec(toRectangle(m_field.startSafeZone), Color{36, 120, 88, 150});
		DrawRectangleRec(toRectangle(m_field.oppositeSafeZone), Color{36, 88, 120, 150});
		DrawRectangleLinesEx(toRectangle(m_field.courtBounds), 2.0f, RAYWHITE);
		drawPolyline(m_field.longitudinalDivider, LIGHTGRAY);
		for (const field::FieldPolyline& line : m_field.transverseLines)
			drawPolyline(line, LIGHTGRAY);

		for (const engine::ecs::Entity defender : m_defenders)
			drawEntityCircle(m_world, defender, MAROON);
		if (!drawEntityCircle(m_world, m_player, m_playerHit ? RED : SKYBLUE))
			return;

		DrawText("SAFE A", static_cast<int>(m_field.startSafeZone.x + 8.0f),
			static_cast<int>(m_field.startSafeZone.y + m_field.startSafeZone.height - 22.0f), 14, RAYWHITE);
		DrawText("SAFE B", static_cast<int>(m_field.oppositeSafeZone.x + 8.0f),
			static_cast<int>(m_field.oppositeSafeZone.y + m_field.oppositeSafeZone.height - 22.0f), 14, RAYWHITE);
		DrawText(TextFormat("Objective: %s", objectiveText(m_roundState)), 10, 10, 16, WHITE);
		DrawText(TextFormat("Round %u | Seed %u", static_cast<unsigned int>(m_roundNumber),
			static_cast<unsigned int>(m_baseSeed)), 10, 50, 14, LIGHTGRAY);
		if (m_playerHit)
			DrawText("PLAYER HIT", 10, 30, 16, RED);
		else
			DrawText("Move: WASD or arrow keys", 10, 30, 16, WHITE);

		if (m_gamePhase == GamePhase::RoundCompleteDelay)
		{
			DrawRectangle(0, 0, static_cast<int>(m_fieldConfig.worldWidth),
				static_cast<int>(m_fieldConfig.worldHeight), Color{0, 0, 0, 150});
			drawCenteredText("ROUND COMPLETE", 138, 28, GREEN, m_fieldConfig.worldWidth);
			drawCenteredText("Preparing next round...", 174, 16, RAYWHITE, m_fieldConfig.worldWidth);
		}
		else if (m_gamePhase == GamePhase::GameOver)
		{
			DrawRectangle(0, 0, static_cast<int>(m_fieldConfig.worldWidth),
				static_cast<int>(m_fieldConfig.worldHeight), Color{0, 0, 0, 190});
			drawCenteredText("CAUGHT", 100, 32, RED, m_fieldConfig.worldWidth);
			drawCenteredText(
				TextFormat("ROUNDS COMPLETED: %u",
					static_cast<unsigned int>(m_completedRounds)),
				150, 18, RAYWHITE, m_fieldConfig.worldWidth
			);
			drawCenteredText(
				TextFormat("HIGH SCORE: %u", static_cast<unsigned int>(m_highScore)),
				178, 18, m_newHighScore ? GOLD : LIGHTGRAY, m_fieldConfig.worldWidth
			);
			if (m_newHighScore)
				drawCenteredText("NEW HIGH SCORE", 202, 16, GOLD, m_fieldConfig.worldWidth);
			drawCenteredText("PRESS ENTER TO PLAY AGAIN", 230, 16,
				RAYWHITE, m_fieldConfig.worldWidth);
			if (m_highScoreSaveFailed)
				drawCenteredText("WARNING: HIGH SCORE COULD NOT BE SAVED", 258, 12,
					ORANGE, m_fieldConfig.worldWidth);
		}
	}
}
