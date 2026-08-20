#pragma once
#ifndef PLAYERCONTROLLER_HPP
#define PLAYERCONTROLLER_HPP

namespace game
{
	struct PlayerController
	{
		float	walkSpeed{};
		float	sprintSpeed{};
		float	exhaustedSpeed{};
	};
}

#endif
