#pragma once
#ifndef GAMEPHASE_HPP
#define GAMEPHASE_HPP

namespace game
{
	enum class GamePhase
	{
		Playing,
		RoundCompleteDelay,
		LoadingNextRound,
		GameOver
	};
}

#endif
